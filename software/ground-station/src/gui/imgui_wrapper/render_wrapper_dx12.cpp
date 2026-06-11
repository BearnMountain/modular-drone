#include "render_wrapper.h"
#include "src/util/logger.h"
#include <imgui/imgui_impl_dx12.h>

namespace Renderer {

struct BackendInitDesc {
};

bool init_from_SDL3(SDL_Window* window, BackendInitDesc* desc) {
	(void)window; (void)desc;

	Log::error("Renderer::init_from_SDL3 dx12 version not created yet");

	return true;
}

void new_frame(SDL_Window* window, BackendInitDesc* desc) {
	(void)window; (void)desc;
	Log::error("Renderer::new_frame dx12 version not created yet");
}

void render(BackendInitDesc* desc) {
	(void)desc;
	Log::error("Renderer::render dx12 version not created yet");
}

void shutdown(void) {
	Log::error("Renderer::shutdown dx12 version not created yet");
}

bool process_event(const SDL_Event* event) {
	(void)event;
	Log::error("Renderer::process_event dx12 version not created yet");
    return true;
}

} // namespace Renderer
