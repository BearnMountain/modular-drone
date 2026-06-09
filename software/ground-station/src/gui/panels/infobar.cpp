#include "infobar.h"

Infobar::Infobar(const std::string name) {
	Log::debug("infobar initialized");

	name_ = name;
}

void Infobar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("App");
	ImGui::End();
}
