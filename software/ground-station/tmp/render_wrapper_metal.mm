// TODO: https://github.com/ocornut/imgui/blob/master/examples/example_glfw_metal/main.mm
// - exampel implementation

// render_wrapper_metal.mm
#define RENDERER_BACKEND_METAL
#include "render_wrapper.h"

#include "src/assets/image_loader.h"

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

// Pull in the real ImGui backend implementations.
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_metal.h>

namespace Renderer {

// We need a way to track the view/layer to fetch the device later for font rebuilding,
// or you can pass the device into RebuildFontAtlas() from your app state. 
// Storing the view locally makes the wrapper self-contained.
static SDL_MetalView g_MetalView = nullptr;
static id<MTLDevice>         s_device;
static CAMetalLayer*         s_layer;
static id<MTLCommandQueue>   s_queue;
id<CAMetalDrawable> 		 drawable;

MTLRenderPassDescriptor* pass;

bool InitFromSDL3(SDL_Window* window, const BackendInitDesc& desc)
{

	if (s_device) {
		Shutdown();
	}
    // 1. Device — create internally if caller didn't provide one
    s_device = desc.device ? desc.device : MTLCreateSystemDefaultDevice();
    if (!s_device) {
        NSLog(@"ERROR: No Metal device");
        return false;
    }

    // 2. Layer — must be set up before ImGui_ImplMetal_Init
    g_MetalView             = SDL_Metal_CreateView(window);
    s_layer                 = (__bridge CAMetalLayer*)SDL_Metal_GetLayer(g_MetalView);
    s_layer.device          = s_device;
    s_layer.pixelFormat     = MTLPixelFormatBGRA8Unorm;
    s_layer.framebufferOnly = YES;

    // 3. Command queue
    s_queue = [s_device newCommandQueue];
    if (!s_queue) {
        NSLog(@"ERROR: Failed to create command queue");
        return false;
    }

    // 4. ImGui — after device and layer are fully configured
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    if (!ImGui_ImplSDL3_InitForMetal(window))
        return false;

	if (!ImGui_ImplMetal_Init(s_device))  // use s_device, not desc.device
		return false; 

	g_MetalDevice = s_device;
    return true;
}

void NewFrame()
{

    drawable 							 = [s_layer nextDrawable];
	if (!drawable) return; // skips to prevent issues
	
    pass 								 = [MTLRenderPassDescriptor renderPassDescriptor];
    pass.colorAttachments[0].texture     = drawable.texture;
    pass.colorAttachments[0].loadAction  = MTLLoadActionClear;
    pass.colorAttachments[0].clearColor  = MTLClearColorMake(0.45, 0.55, 0.60, 1.0);
    pass.colorAttachments[0].storeAction = MTLStoreActionStore;

    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplMetal_NewFrame(pass);
    ImGui::NewFrame();
}

void Render()
{
    ImGui::Render();
    
    id<MTLCommandBuffer>        cmd     = [s_queue commandBuffer];
    id<MTLRenderCommandEncoder> encoder = [cmd renderCommandEncoderWithDescriptor:pass];

    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), cmd, encoder);

    [encoder endEncoding];
    [cmd presentDrawable:drawable];
    [cmd commit];
}

void Shutdown()
{
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    
    if (g_MetalView) {
        SDL_Metal_DestroyView(g_MetalView);
        g_MetalView = nullptr;
    }
    
    ImGui::DestroyContext();
}

bool ProcessEvent(const SDL_Event* event)
{
    return ImGui_ImplSDL3_ProcessEvent(event);
}

// FIX 2: Retrieve the device context from our active layer to rebuild objects
void RebuildFontAtlas()
{
    if (!g_MetalView) return;
    
    CAMetalLayer* metal_layer = (__bridge CAMetalLayer*)SDL_Metal_GetLayer(g_MetalView);
    id<MTLDevice> device = metal_layer.device;

    ImGui_ImplMetal_DestroyDeviceObjects();
    ImGui_ImplMetal_CreateDeviceObjects(device);
}

} // namespace Renderer
