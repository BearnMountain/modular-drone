// render_wrapper_vulkan.cpp
// Compiled only when RENDERER_BACKEND_VULKAN is defined.

#define RENDERER_BACKEND_VULKAN
#include "render_wrapper.h"

// Pull in the real ImGui backend implementations.
// Remove these if you compile them as separate translation units.
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_vulkan.h>

namespace Renderer {

bool InitFromSDL3(SDL_Window* window, const BackendInitDesc& desc)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // SDL3 platform layer
    if (!ImGui_ImplSDL3_InitForVulkan(window))
        return false;

    // Vulkan renderer layer
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance        = desc.instance;
    init_info.PhysicalDevice  = desc.physical_device;
    init_info.Device          = desc.device;
    init_info.QueueFamily     = desc.queue_family;
    init_info.Queue           = desc.queue;
    init_info.DescriptorPool  = desc.descriptor_pool;
    init_info.RenderPass      = desc.render_pass;
    init_info.MinImageCount   = desc.min_image_count;
    init_info.ImageCount      = desc.image_count;
    init_info.MSAASamples     = desc.msaa_samples;

    if (!ImGui_ImplVulkan_Init(&init_info))
        return false;

    return true;
}

void NewFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void Render()
{
	// TODO
	const FrameContext ctx;
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ctx.command_buffer);
}

void Shutdown()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

bool ProcessEvent(const SDL_Event* event)
{
    return ImGui_ImplSDL3_ProcessEvent(event);
}

void RebuildFontAtlas()
{
    // Vulkan: upload fonts via a one-shot command buffer.
    // Caller is responsible for queue idle before/after if needed.
    ImGui_ImplVulkan_CreateFontsTexture();
}

} // namespace Renderer
