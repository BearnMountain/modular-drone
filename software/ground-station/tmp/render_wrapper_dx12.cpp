// render_wrapper_dx12.cpp
// Compiled only when RENDERER_BACKEND_DX12 is defined.
// Windows only.

#define RENDERER_BACKEND_DX12
#include "render_wrapper.h"

// Pull in the real ImGui backend implementations.
// Remove these if you compile them as separate translation units.
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_dx12.h>

namespace Renderer {

bool InitFromSDL3(SDL_Window* window, const BackendInitDesc& desc)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // SDL3 platform layer
    if (!ImGui_ImplSDL3_InitForD3D(window))
        return false;

    // DX12 renderer layer
    if (!ImGui_ImplDX12_Init(
            desc.device,
            desc.num_frames_in_flight,
            desc.rtv_format,
            desc.cbv_srv_heap,
            desc.font_srv_cpu_handle,
            desc.font_srv_gpu_handle))
        return false;

    return true;
}

void NewFrame()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void Render()
{
	// TODO
	const FrameContext ctx;
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), ctx.command_list);
}

void Shutdown()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

bool ProcessEvent(const SDL_Event* event)
{
    return ImGui_ImplSDL3_ProcessEvent(event);
}

void RebuildFontAtlas()
{
    ImGui_ImplDX12_InvalidateDeviceObjects();
    ImGui_ImplDX12_CreateDeviceObjects();
}

} // namespace Renderer
