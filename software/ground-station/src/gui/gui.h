#pragma once

#include <SDL3/SDL.h>

class GUI {
public:
	GUI(SDL_Window* window);
	~GUI();
	void draw(void);
private:
};
