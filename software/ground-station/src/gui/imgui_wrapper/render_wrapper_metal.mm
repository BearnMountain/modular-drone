#include <implot3d/implot3d.h>
#include "render_wrapper.h"
#include <imgui/imgui_impl_metal.h>
#include "src/util/logger.h"

#include <SDL3/SDL_metal.h>
#include "backend_metal_defines.h"

namespace Renderer {

static float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};

BackendInitDesc* alloc_desc() {
    return new BackendInitDesc{};
}

void free_desc(BackendInitDesc* desc) {
    delete desc;
}

bool init_from_SDL3(SDL_Window* window, BackendInitDesc* desc) {
    id<MTLDevice> metal_device = MTLCreateSystemDefaultDevice();
    if (!metal_device) {
        Log::fatal("failed to create metal device");
        return false;
    }
    SDL_MetalView view = SDL_Metal_CreateView(window);
    desc->layer = (__bridge CAMetalLayer*)SDL_Metal_GetLayer(view);
    desc->layer.device = metal_device;
    desc->layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    desc->command_queue = [desc->layer.device newCommandQueue];
    desc->render_pass_descriptor = [MTLRenderPassDescriptor new];

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	ImPlot3D::CreateContext();
    ImGui_ImplMetal_Init(desc->layer.device);
    ImGui_ImplSDL3_InitForMetal(window);
    return true;                                  // fix #1
}

void new_frame(SDL_Window* window, BackendInitDesc* desc) {
    int width, height;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    desc->layer.drawableSize = CGSizeMake(width, height);

    desc->drawable     = [desc->layer nextDrawable];                          // fix #5
    desc->command_buffer = [desc->command_queue commandBuffer];               // fix #2

    desc->render_pass_descriptor.colorAttachments[0].clearColor =
        MTLClearColorMake(clear_color[0] * clear_color[3],
                          clear_color[1] * clear_color[3],
                          clear_color[2] * clear_color[3],
                          clear_color[3]);
    desc->render_pass_descriptor.colorAttachments[0].texture     = desc->drawable.texture;
    desc->render_pass_descriptor.colorAttachments[0].loadAction  = MTLLoadActionClear;
    desc->render_pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

    desc->render_encoder = [desc->command_buffer                              // fix #3
        renderCommandEncoderWithDescriptor:desc->render_pass_descriptor];
    [desc->render_encoder pushDebugGroup:@"ImGui demo"];                      // fix #4

    ImGui_ImplMetal_NewFrame(desc->render_pass_descriptor);
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void render(BackendInitDesc* desc) {
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    ImGui_ImplMetal_RenderDrawData(draw_data, desc->command_buffer, desc->render_encoder);
    [desc->render_encoder popDebugGroup];
    [desc->render_encoder endEncoding];
    [desc->command_buffer presentDrawable:desc->drawable];                    // fix #5
    [desc->command_buffer commit];
}

void shutdown(void) {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplSDL3_Shutdown();
	ImPlot3D::DestroyContext();
    ImGui::DestroyContext();
}

bool process_event(const SDL_Event* event) {
    return ImGui_ImplSDL3_ProcessEvent(event);
}

} // namespace Renderer
