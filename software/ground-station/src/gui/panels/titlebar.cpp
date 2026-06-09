#include "titlebar.h" 

Titlebar::Titlebar(const std::string name) {
	Log::debug("titlebar initialized");

	name_ = name;
}

void Titlebar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("App");
	ImGui::End();
}
