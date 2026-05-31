#include <iostream>

#define SDL_MAIN_USE_CALLBACK
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "util/defines.h"
#include "util/logger.h"

static SDL_Window* window;
static u32 window_width, window_height;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	(void)argc; (void)argv; (void)appstate;
	
#if DEBUG
	SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
#endif

	window_width = 960;
	window_height = 540;
	window = SDL_CreateWindow("App", window_width, window_height, SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		log_err("failed to initialize SDL window: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// gui startup


	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	(void)appstate;



	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	(void)appstate;

	switch (event->type) {
		case SDL_EVENT_KEY_DOWN: {
			if (event->key.key == SDLK_Q || event->key.key == SDLK_ESCAPE) {
				return SDL_APP_SUCCESS;
			}
			
			switch(event->key.key) {
				case SDLK_1:
					break;
				case SDLK_2:
					break;
				case SDLK_3:
					break;
				case SDLK_4:
					break;
				case SDLK_5: {
					break;
				}
			}
		}
		default: break;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	(void)appstate;
	(void)result;

	SDL_DestroyWindow(window);
	SDL_Quit();
}

