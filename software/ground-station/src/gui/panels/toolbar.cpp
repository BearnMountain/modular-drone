#include "toolbar.h"

Toolbar::Toolbar(const std::string name) {
	Log::debug("initialized toolbar");

	name_ = name;
}


void Toolbar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("Infographics / 3D Scene Go Here");
	ImGui::End();
}
