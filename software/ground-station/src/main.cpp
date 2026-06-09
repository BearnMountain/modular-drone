#include "src/gui/gui.h"
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <iostream>

#include "util/defines.h"
#include "util/logger.h"

// class GroundStation {
// public:
//     // bool init();
//     // void update();
//     // void handleEvent(const SDL_Event& event);
//     // void cleanUp();
// 	void init();
//
// 	inline const GUI* getGUI(void) const { return gui; }
// private: 
// 	GUI* gui;
// };

struct AppData {
	GUI* gui;
};

static SDL_Window* window;
static u32 window_width, window_height;

extern "C" {

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	(void)argc; (void)argv;
	window_width = 800;
	window_height = 500;
	window = SDL_CreateWindow("App", window_width, window_height, 0);
	if (!window) {
		Log::fatal("failed to create window instance");
		return SDL_APP_FAILURE;
	}
	
	// window configurations
	SDL_RaiseWindow(window);
	
	AppData* app_data = new AppData();
	if (!app_data) {
		return SDL_APP_FAILURE;
	}

	app_data->gui = new GUI(window, window_width, window_height);
	*appstate = app_data;


    // GroundStation* app = new GroundStation();
    // if (!app->init()) {
    //     delete app;
    //     return SDL_APP_FAILURE;
    // }
    // *appstate = app; // Save our C++ object instance pointer
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {

    AppData* app = static_cast<AppData*>(appstate);
	app->gui->draw();

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
    delete app_data->gui;
	delete app_data;

	SDL_DestroyWindow(window);
	SDL_Quit();
}

} // extern "C"

