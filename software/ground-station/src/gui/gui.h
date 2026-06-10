#pragma once

#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <memory>

#include "src/gui/panels/viewport.h"
#include "src/gui/panels/infobar.h"
#include "src/gui/panels/navbar.h"
#include "src/gui/panels/statusbar.h"
#include "src/gui/panels/titlebar.h"
#include "src/gui/panels/toolbar.h"

class GUI {
public:
    GUI(SDL_Window* window, f32 window_width, f32 window_height);
    ~GUI();
    void draw(void);
    void event_handler(SDL_Event* event);

private:

	f32 x_, y_, width_, height_;
	std::unique_ptr<Viewport> viewport;
	std::unique_ptr<Navbar> navbar;
	std::unique_ptr<Infobar> infobar;
	std::unique_ptr<Titlebar> titlebar;
	std::unique_ptr<Statusbar> statusbar;
	std::unique_ptr<Toolbar> toolbar;

};
