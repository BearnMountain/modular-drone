#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "util/logger.h"

#include "src/gui/gui.h"
#include "src/util/config.h"

struct AppData {
	SDL_Window* window;
	std::unique_ptr<GUI> gui;
};

extern "C" {

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	(void)argc; (void)argv;

	auto app_data = std::make_unique<AppData>();
	if (!app_data) {
		Log::error("failed to initialize AppData");
		return SDL_APP_FAILURE;
	}
	
	SDL_WindowFlags window_flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
#ifdef RENDERER_BACKEND_METAL
	window_flags |= SDL_WINDOW_METAL;
#elif RENDERER_BACKEND_VULKAN
	window_flags |= SDL_WINDOW_VULKAN;
#endif
	app_data->window = SDL_CreateWindow("App", Config::window_width, Config::window_height, window_flags);
	if (!app_data->window) {
		Log::fatal("failed to create window instance: {}", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	// window configurations
	SDL_SetWindowPosition(app_data->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(app_data->window);
	SDL_RaiseWindow(app_data->window);
	
	app_data->gui = std::make_unique<GUI>(
		app_data->window, 
		Config::window_width, 
		Config::window_height
	);

	*appstate = app_data.release();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {

    AppData* app = static_cast<AppData*>(appstate);
	app->gui->draw(app->window);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    AppData* app = static_cast<AppData*>(appstate);
    app->gui->event_handler(event);


	switch (event->type) {
		case SDL_EVENT_KEY_DOWN: {
			if (event->key.key == SDLK_Q || event->key.key == SDLK_ESCAPE) {
				return SDL_APP_SUCCESS;
			}
			break;
		}
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			return SDL_APP_SUCCESS;

		default: break;
	}
	

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    (void)result;
    AppData* app_data = static_cast<AppData*>(appstate);
    if (!app_data) return;
    SDL_DestroyWindow(app_data->window);
    app_data->window = nullptr;
    delete app_data; 
    SDL_Quit();
}

} // extern "C"

