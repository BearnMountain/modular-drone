#pragma once

// render_wrapper.h
// Unified ImGui renderer abstraction for Metal, Vulkan, and DirectX 12.
// Include this header anywhere you need renderer-agnostic access.
// The correct backend impl is selected at compile time via preprocessor defines:
//
//   RENDERER_BACKEND_METAL   -> imgui_impl_metal.mm  + imgui_impl_sdl3.cpp
//   RENDERER_BACKEND_VULKAN  -> imgui_impl_vulkan.cpp + imgui_impl_sdl3.cpp
//   RENDERER_BACKEND_DX12    -> imgui_impl_dx12.cpp  + imgui_impl_sdl3.cpp
//
// Usage:
//   Renderer::InitFromSDL3(window, ...);
//   while (running) {
//       Renderer::NewFrame();
//       // ImGui calls …
//       Renderer::Render();
//   }
//   Renderer::Shutdown();

// ── Backend detection ────────────────────────────────────────────────────────
#if !defined(RENDERER_BACKEND_METAL) && \
    !defined(RENDERER_BACKEND_VULKAN) && \
    !defined(RENDERER_BACKEND_DX12)
#  if defined(__APPLE__)
#    define RENDERER_BACKEND_METAL
#  elif defined(_WIN32)
#    define RENDERER_BACKEND_DX12
#  else
#    define RENDERER_BACKEND_VULKAN
#  endif
#endif

// guards against vulkan inclusion
#if !defined(RENDERER_BACKEND_VULKAN)
#define SDL_DISABLE_VULKAN  // tells SDL3 not to include vulkan.h
#endif

#if !defined(RENDERER_BACKEND_METAL)
#define SDL_DISABLE_METAL  // tells SDL3 not to include vulkan.h
#endif

#if !defined(RENDERER_BACKEND_DX12)
#define SDL_DISABLE_DX12  // tells SDL3 not to include vulkan.h
#endif

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>

#include <SDL3/SDL.h>
#include <cstdint>



// ── Backend-specific includes ────────────────────────────────────────────────
#if defined(RENDERER_BACKEND_METAL)
#include <imgui/imgui_impl_metal.h>
    // MTLDevice / CAMetalLayer are Obj-C types; forward-declare for C++ TUs.
#ifdef __OBJC__
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#else
    // Opaque pointers so C++ translation units can hold handles
    // without importing Objective-C headers.
    typedef struct objc_object MTLDevice_t;
    typedef struct objc_object CAMetalLayer_t;
    typedef struct objc_object MTLCommandBuffer_t;
    typedef struct objc_object MTLRenderPassDescriptor_t;
    typedef struct objc_object MTLRenderCommandEncoder_t;
#endif

#elif defined(RENDERER_BACKEND_VULKAN)
#include <vulkan/vulkan.h>
#include "imgui_impl_vulkan.h"

#elif defined(RENDERER_BACKEND_DX12)
#include <d3d12.h>
#include <dxgi1_4.h>
#include "imgui_impl_dx12.h"
#endif

// ── Init descriptors ─────────────────────────────────────────────────────────

namespace Renderer {

#if defined(RENDERER_BACKEND_METAL)

struct BackendInitDesc {
#  ifdef __OBJC__
    id<MTLDevice> device = nil;
    CAMetalLayer* layer  = nil;
#  else
    MTLDevice_t*     device = nullptr;
    CAMetalLayer_t*  layer  = nullptr;
#  endif
    int         num_frames_in_flight = 3;   // typically 2-3 for Metal
};

// Per-frame data the caller must fill before calling Render()
struct FrameContext {
#  ifdef __OBJC__
    id<MTLCommandBuffer>          command_buffer   = nil;
    MTLRenderPassDescriptor*      render_pass_desc = nil;
    id<MTLRenderCommandEncoder>   encoder          = nil;
#  else
    MTLCommandBuffer_t*           command_buffer   = nullptr;
    MTLRenderPassDescriptor_t*    render_pass_desc = nullptr;
    MTLRenderCommandEncoder_t*    encoder          = nullptr;
#  endif
};

#elif defined(RENDERER_BACKEND_VULKAN)

struct BackendInitDesc {
    VkInstance               instance        = VK_NULL_HANDLE;
    VkPhysicalDevice         physical_device = VK_NULL_HANDLE;
    VkDevice                 device          = VK_NULL_HANDLE;
    VkQueue                  queue           = VK_NULL_HANDLE;
    uint32_t                 queue_family    = 0;
    VkDescriptorPool         descriptor_pool = VK_NULL_HANDLE;
    VkRenderPass             render_pass     = VK_NULL_HANDLE;
    uint32_t                 min_image_count = 2;
    uint32_t                 image_count     = 2;
    VkSampleCountFlagBits    msaa_samples    = VK_SAMPLE_COUNT_1_BIT;
};

struct FrameContext {
    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
};

#elif defined(RENDERER_BACKEND_DX12)

struct BackendInitDesc {
    ID3D12Device*              device               = nullptr;
    int                        num_frames_in_flight = 2;
    DXGI_FORMAT                rtv_format           = DXGI_FORMAT_R8G8B8A8_UNORM;
    ID3D12DescriptorHeap*      cbv_srv_heap         = nullptr;   // shader-visible SRV heap
    D3D12_CPU_DESCRIPTOR_HANDLE font_srv_cpu_handle = {};
    D3D12_GPU_DESCRIPTOR_HANDLE font_srv_gpu_handle = {};
};

struct FrameContext {
    ID3D12GraphicsCommandList* command_list    = nullptr;
    ID3D12CommandAllocator*    command_alloc   = nullptr;
};

#endif // backend

// ── Core API ─────────────────────────────────────────────────────────────────

/// Call once at startup.
/// @param window   The SDL3 window (imgui_impl_sdl3 is always initialised here).
/// @param desc     Backend-specific initialisation parameters (see above).
/// @return         true on success.
bool InitFromSDL3(SDL_Window* window, const BackendInitDesc& desc);

/// Call at the start of each frame (before any ImGui:: calls).
void NewFrame();

/// Call after all ImGui:: calls, before your command buffer / present.
/// For Metal/DX12 you pass a filled-in FrameContext; for Vulkan the
/// command buffer lives in FrameContext::command_buffer.
void Render();

/// Frees all ImGui backend resources. Does NOT destroy your device/swapchain.
void Shutdown();

/// Forwards an SDL_Event to imgui_impl_sdl3.
/// Returns true if ImGui consumed the event.
bool ProcessEvent(const SDL_Event* event);

/// Convenience: upload fonts / rebuild the font atlas.
/// Call after InitFromSDL3 and after any AddFont*() calls.
void RebuildFontAtlas();

} // namespace Renderer
