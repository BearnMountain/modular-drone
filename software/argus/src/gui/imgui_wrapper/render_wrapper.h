#pragma once

/*
	Cross platform render wrapper for imgui:
	 - Metal(Macos)
	 - DX12(Window)
	 - Vulkan(Linux/Default for others)
*/

#include "imgui/imgui_impl_sdl3.h"
#include <SDL3/SDL.h>

// #if defined(RENDERER_METAL)
// #elif defined(RENDERER_VULKAN)
// #elif defined(RENDERER_DX12)
// #endif

namespace Renderer {

	// opaque descriptor for cross platform backend graphic libraries
	struct BackendInitDesc;
	BackendInitDesc* alloc_desc(void);
	void free_desc(BackendInitDesc* desc);

	// Call once at startup
	bool init_from_SDL3(SDL_Window* window, BackendInitDesc* desc);

	// Call for each render pass
	void new_frame(SDL_Window* window, BackendInitDesc* desc);

	// Completes pass and sends to SDL_Window
	void render(BackendInitDesc* desc);

	// Cleans up ImGui backend resources
	void shutdown(void);

	// Forwards events to imgui
	bool process_event(const SDL_Event* event);
}
