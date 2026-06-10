// imgui_texture_loader.cpp
// Vulkan + Direct3D 12 backends.
// For Metal, compile imgui_texture_loader.mm instead.
//
// Define exactly one of:
//   IMGUI_BACKEND_VULKAN
//   IMGUI_BACKEND_DX12

#include "image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cassert>
#include <cstdio>
#include <cstring>

static uint32_t s_last_w = 0;
static uint32_t s_last_h = 0;

void last_texture_size(uint32_t* out_w, uint32_t* out_h) {
    if (out_w) *out_w = s_last_w;
    if (out_h) *out_h = s_last_h;
}

// ============================================================================
//  VULKAN
// ============================================================================
#if defined(IMGUI_BACKEND_VULKAN)

#include "imgui_impl_vulkan.h"
#include <vulkan/vulkan.h>

// Wire these up from your engine's Vulkan context:
extern VkDevice          g_Device;
extern VkPhysicalDevice  g_PhysicalDevice;
extern VkQueue           g_GraphicsQueue;
extern VkCommandPool     g_CommandPool;
extern VkDescriptorPool  g_DescriptorPool; // must allow COMBINED_IMAGE_SAMPLER

namespace {

struct VkTexture {
    VkImage         image   = VK_NULL_HANDLE;
    VkDeviceMemory  memory  = VK_NULL_HANDLE;
    VkImageView     view    = VK_NULL_HANDLE;
    VkSampler       sampler = VK_NULL_HANDLE;
    VkDescriptorSet ds      = VK_NULL_HANDLE;
    uint32_t w = 0, h = 0;
};

uint32_t find_memory_type(uint32_t type_bits, VkMemoryPropertyFlags props) {
    VkPhysicalDeviceMemoryProperties mem;
    vkGetPhysicalDeviceMemoryProperties(g_PhysicalDevice, &mem);
    for (uint32_t i = 0; i < mem.memoryTypeCount; ++i)
        if ((type_bits & (1u << i)) &&
            (mem.memoryTypes[i].propertyFlags & props) == props)
            return i;
    assert(!"No suitable memory type");
    return UINT32_MAX;
}

void upload_via_staging(VkBuffer staging, VkImage dst, uint32_t w, uint32_t h) {
    VkCommandBufferAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool        = g_CommandPool;
    ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = 1;

    VkCommandBuffer cb;
    vkAllocateCommandBuffers(g_Device, &ai, &cb);

    VkCommandBufferBeginInfo bi{};
    bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cb, &bi);

    // UNDEFINED → TRANSFER_DST
    VkImageMemoryBarrier barrier{};
    barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image               = dst;
    barrier.subresourceRange    = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    barrier.srcAccessMask       = 0;
    barrier.dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT;
    vkCmdPipelineBarrier(cb,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0, 0, nullptr, 0, nullptr, 1, &barrier);

    VkBufferImageCopy region{};
    region.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    region.imageExtent      = {w, h, 1};
    vkCmdCopyBufferToImage(cb, staging, dst,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    // TRANSFER_DST → SHADER_READ_ONLY
    barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vkCmdPipelineBarrier(cb,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, nullptr, 0, nullptr, 1, &barrier);

    vkEndCommandBuffer(cb);

    VkSubmitInfo si{};
    si.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    si.commandBufferCount = 1;
    si.pCommandBuffers    = &cb;
    vkQueueSubmit(g_GraphicsQueue, 1, &si, VK_NULL_HANDLE);
    vkQueueWaitIdle(g_GraphicsQueue);
    vkFreeCommandBuffers(g_Device, g_CommandPool, 1, &cb);
}

} // namespace

ImTextureID load_imgui_texture(const char* path) {
    int w, h, ch;
    stbi_uc* pixels = stbi_load(path, &w, &h, &ch, STBI_rgb_alpha);
    if (!pixels) {
        fprintf(stderr, "[load_texture] stbi_load failed: %s\n", path);
        return nullptr;
    }
    const VkDeviceSize size = (VkDeviceSize)w * h * 4;

    auto* tex = new VkTexture();
    tex->w = (uint32_t)w;
    tex->h = (uint32_t)h;

    // ── Staging buffer ───────────────────────────────────────────────────────
    VkBuffer       staging_buf;
    VkDeviceMemory staging_mem;
    {
        VkBufferCreateInfo bi{};
        bi.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bi.size        = size;
        bi.usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkCreateBuffer(g_Device, &bi, nullptr, &staging_buf);

        VkMemoryRequirements req;
        vkGetBufferMemoryRequirements(g_Device, staging_buf, &req);
        VkMemoryAllocateInfo ai{};
        ai.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        ai.allocationSize  = req.size;
        ai.memoryTypeIndex = find_memory_type(req.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        vkAllocateMemory(g_Device, &ai, nullptr, &staging_mem);
        vkBindBufferMemory(g_Device, staging_buf, staging_mem, 0);

        void* mapped;
        vkMapMemory(g_Device, staging_mem, 0, size, 0, &mapped);
        memcpy(mapped, pixels, (size_t)size);
        vkUnmapMemory(g_Device, staging_mem);
    }
    stbi_image_free(pixels);

    // ── Device image ─────────────────────────────────────────────────────────
    {
        VkImageCreateInfo ci{};
        ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        ci.imageType     = VK_IMAGE_TYPE_2D;
        ci.format        = VK_FORMAT_R8G8B8A8_UNORM;
        ci.extent        = {(uint32_t)w, (uint32_t)h, 1};
        ci.mipLevels     = 1;
        ci.arrayLayers   = 1;
        ci.samples       = VK_SAMPLE_COUNT_1_BIT;
        ci.tiling        = VK_IMAGE_TILING_OPTIMAL;
        ci.usage         = VK_IMAGE_USAGE_SAMPLED_BIT |
                           VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
        ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vkCreateImage(g_Device, &ci, nullptr, &tex->image);

        VkMemoryRequirements req;
        vkGetImageMemoryRequirements(g_Device, tex->image, &req);
        VkMemoryAllocateInfo ai{};
        ai.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        ai.allocationSize  = req.size;
        ai.memoryTypeIndex = find_memory_type(req.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkAllocateMemory(g_Device, &ai, nullptr, &tex->memory);
        vkBindImageMemory(g_Device, tex->image, tex->memory, 0);
    }

    upload_via_staging(staging_buf, tex->image, (uint32_t)w, (uint32_t)h);
    vkDestroyBuffer(g_Device, staging_buf, nullptr);
    vkFreeMemory(g_Device, staging_mem, nullptr);

    // ── Image view ───────────────────────────────────────────────────────────
    {
        VkImageViewCreateInfo vi{};
        vi.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        vi.image            = tex->image;
        vi.viewType         = VK_IMAGE_VIEW_TYPE_2D;
        vi.format           = VK_FORMAT_R8G8B8A8_UNORM;
        vi.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        vkCreateImageView(g_Device, &vi, nullptr, &tex->view);
    }

    // ── Sampler ──────────────────────────────────────────────────────────────
    {
        VkSamplerCreateInfo si{};
        si.sType         = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        si.magFilter     = VK_FILTER_LINEAR;
        si.minFilter     = VK_FILTER_LINEAR;
        si.mipmapMode    = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        si.addressModeU  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        si.addressModeV  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        si.addressModeW  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        si.maxAnisotropy = 1.0f;
        si.maxLod        = VK_LOD_CLAMP_NONE;
        vkCreateSampler(g_Device, &si, nullptr, &tex->sampler);
    }

    // ── Register with ImGui ──────────────────────────────────────────────────
    tex->ds = ImGui_ImplVulkan_AddTexture(
        tex->sampler, tex->view,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    s_last_w = tex->w;
    s_last_h = tex->h;
    return (ImTextureID)tex->ds;
}

void free_texture(ImTextureID id) {
    if (!id) return;
    // GPU must be idle before destroying resources.
    // Retrieve VkTexture* from your parallel ownership table, then:
    //   vkDeviceWaitIdle(g_Device);
    //   ImGui_ImplVulkan_RemoveTexture(tex->ds);
    //   vkDestroySampler / vkDestroyImageView / vkDestroyImage / vkFreeMemory
    //   delete tex;
    (void)id;
}

// ============================================================================
//  DIRECT3D 12
// ============================================================================
#elif defined(IMGUI_BACKEND_DX12)

#include <d3d12.h>
#include <wrl/client.h>
#include "imgui_impl_dx12.h"

using Microsoft::WRL::ComPtr;

// Wire these up from your engine:
extern ID3D12Device*              g_D3D12Device;
extern ID3D12CommandQueue*        g_D3D12CommandQueue;
extern ID3D12GraphicsCommandList* g_D3D12CommandList;  // open, ready to record
extern ID3D12CommandAllocator*    g_D3D12CmdAllocator;

// Carve slots from the SRV heap passed to ImGui_ImplDX12_Init:
extern D3D12_CPU_DESCRIPTOR_HANDLE AllocSrvCpuHandle();
extern D3D12_GPU_DESCRIPTOR_HANDLE AllocSrvGpuHandle();

namespace {

void execute_and_wait(ID3D12CommandList* cl) {
    ID3D12CommandList* lists[] = {cl};
    g_D3D12CommandQueue->ExecuteCommandLists(1, lists);

    ComPtr<ID3D12Fence> fence;
    g_D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    HANDLE ev = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    g_D3D12CommandQueue->Signal(fence.Get(), 1);
    fence->SetEventOnCompletion(1, ev);
    WaitForSingleObject(ev, INFINITE);
    CloseHandle(ev);
}

} // namespace

ImTextureID load_imgui_texture(const char* path) {
    int w, h, ch;
    stbi_uc* pixels = stbi_load(path, &w, &h, &ch, STBI_rgb_alpha);
    if (!pixels) {
        fprintf(stderr, "[load_texture] stbi_load failed: %s\n", path);
        return nullptr;
    }
    const UINT64 row_pitch   = (UINT64)w * 4;
    const UINT64 upload_size = row_pitch * (UINT64)h;

    // ── Default-heap texture (GPU) ────────────────────────────────────────────
    ComPtr<ID3D12Resource> texture;
    {
        D3D12_HEAP_PROPERTIES hp{D3D12_HEAP_TYPE_DEFAULT};
        D3D12_RESOURCE_DESC   rd{};
        rd.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        rd.Width            = (UINT64)w;
        rd.Height           = (UINT)h;
        rd.DepthOrArraySize = 1;
        rd.MipLevels        = 1;
        rd.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
        rd.SampleDesc       = {1, 0};
        rd.Layout           = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        rd.Flags            = D3D12_RESOURCE_FLAG_NONE;
        g_D3D12Device->CreateCommittedResource(
            &hp, D3D12_HEAP_FLAG_NONE, &rd,
            D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
            IID_PPV_ARGS(&texture));
    }

    // ── Upload-heap staging buffer ────────────────────────────────────────────
    ComPtr<ID3D12Resource> upload;
    {
        D3D12_HEAP_PROPERTIES hp{D3D12_HEAP_TYPE_UPLOAD};
        D3D12_RESOURCE_DESC   rd = CD3DX12_RESOURCE_DESC::Buffer(upload_size);
        g_D3D12Device->CreateCommittedResource(
            &hp, D3D12_HEAP_FLAG_NONE, &rd,
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&upload));

        void* mapped;
        upload->Map(0, nullptr, &mapped);
        memcpy(mapped, pixels, (size_t)upload_size);
        upload->Unmap(0, nullptr);
    }
    stbi_image_free(pixels);

    // ── Copy + transition ─────────────────────────────────────────────────────
    g_D3D12CmdAllocator->Reset();
    g_D3D12CommandList->Reset(g_D3D12CmdAllocator, nullptr);
    {
        D3D12_TEXTURE_COPY_LOCATION dst{};
        dst.pResource        = texture.Get();
        dst.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        dst.SubresourceIndex = 0;

        D3D12_TEXTURE_COPY_LOCATION src{};
        src.pResource                          = upload.Get();
        src.Type                               = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        src.PlacedFootprint.Footprint.Format   = DXGI_FORMAT_R8G8B8A8_UNORM;
        src.PlacedFootprint.Footprint.Width    = (UINT)w;
        src.PlacedFootprint.Footprint.Height   = (UINT)h;
        src.PlacedFootprint.Footprint.Depth    = 1;
        src.PlacedFootprint.Footprint.RowPitch = (UINT)row_pitch;
        g_D3D12CommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource   = texture.Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        g_D3D12CommandList->ResourceBarrier(1, &barrier);
    }
    g_D3D12CommandList->Close();
    execute_and_wait(g_D3D12CommandList);

    // ── SRV descriptor ────────────────────────────────────────────────────────
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = AllocSrvCpuHandle();
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle = AllocSrvGpuHandle();

    D3D12_SHADER_RESOURCE_VIEW_DESC srv{};
    srv.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    srv.ViewDimension             = D3D12_SRV_DIMENSION_TEXTURE2D;
    srv.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv.Texture2D.MipLevels       = 1;
    g_D3D12Device->CreateShaderResourceView(texture.Get(), &srv, cpu_handle);

    texture.Detach(); // transfer ownership to your resource table

    s_last_w = (uint32_t)w;
    s_last_h = (uint32_t)h;
    return (ImTextureID)gpu_handle.ptr;
}

void free_texture(ImTextureID id) {
    // Look up the ID3D12Resource by gpu_handle.ptr in your table,
    // wait for GPU idle, then Release() it and free the descriptor slot.
    (void)id;
}

#else
// #  error "Define IMGUI_BACKEND_VULKAN or IMGUI_BACKEND_DX12 when compiling this file. \
// For Metal, compile imgui_texture_loader.mm instead."
#endif
