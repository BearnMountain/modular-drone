#include "statusbar.h"

Statusbar::Statusbar(const std::string name) {
	Log::debug("statusbar initialized");

	name_ = name;
}

void Statusbar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("App");
	ImGui::End();
}
