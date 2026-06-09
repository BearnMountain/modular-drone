#include "viewport.h"
#include "src/util/logger.h"

Viewport::Viewport(const std::string name) {
	Log::debug("viewport initialized");

	name_ = name;
}


void Viewport::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("Infographics / 3D Scene Go Here");
	ImGui::End();
}
