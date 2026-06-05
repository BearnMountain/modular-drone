#include "gui.h"
#include <imgui/imgui.h>
#include "imgui_wrapper/render_wrapper.h"

GUI::GUI(SDL_Window* window) {
	Renderer::BackendInitDesc desc;
	Renderer::InitFromSDL3(window, desc);  

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
}

GUI::~GUI() {
	Renderer::Shutdown();
}

void GUI::draw(void) {
	Renderer::NewFrame();

    ImGui::Begin("Demo window");
    ImGui::Button("Hello!");
    ImGui::End();

	Renderer::Render();
} 

