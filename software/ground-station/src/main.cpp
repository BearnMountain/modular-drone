#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <iostream>

#include "util/defines.h"
#include "util/logger.h"

class GroundStation {
public:
    bool init();
    void update();
    void handleEvent(const SDL_Event& event);
    void cleanUp();
};

static SDL_Window* window;
static u32 window_width, window_height;

extern "C" {

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	(void)appstate; (void)argc; (void)argv;
	window_width = 900;
	window_height = 800;
	window = SDL_CreateWindow("App", window_width, window_height, 0);
	if (!window) {
		Log::fatal("failed to create window instance");
		return SDL_APP_FAILURE;
	}

	Log::fatal("test");
	Log::debug("test");
	Log::info("test");
	Log::error("test");
	Log::warn("test");

	SDL_RaiseWindow(window);

    // GroundStation* app = new GroundStation();
    // if (!app->init()) {
    //     delete app;
    //     return SDL_APP_FAILURE;
    // }
    // *appstate = app; // Save our C++ object instance pointer
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	(void)appstate;
    // auto* app = static_cast<GroundStation*>(appstate);
    // app->update();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	(void)appstate;
    // auto* app = static_cast<GroundStation*>(appstate);
    // app->handleEvent(*event);


	switch (event->type) {
		case SDL_EVENT_KEY_DOWN: {
			if (event->key.key == SDLK_Q || event->key.key == SDLK_ESCAPE) {
				return SDL_APP_SUCCESS;
			}
		}
		default: break;
	}
	

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	(void)appstate; (void)result;
    // auto* app = static_cast<GroundStation*>(appstate);
    // if (app) {
    //     app->cleanUp();;
    //     delete app;
    // }

	SDL_DestroyWindow(window);
	SDL_Quit();
}

} // extern "C"

