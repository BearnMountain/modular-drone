#include "navbar.h"

Navbar::Navbar(const std::string name) {
	Log::debug("navbar initialized");

	name_ = name;
}

void Navbar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("App");
	ImGui::End();
}
