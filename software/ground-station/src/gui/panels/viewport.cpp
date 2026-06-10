#include "viewport.h"
#include "src/util/logger.h"

Viewport::Viewport(const std::string name) {
	Log::debug("viewport initialized");

	name_ = name;
	id_ = 0;
}


void Viewport::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("Viewport");
	ImGui::End();
}

void Viewport::configure() {
	if (!id_) {
		Log::debug("viewport configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_))
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	else 
		Log::error("ImGuiID '{}' doesn't exist(Viewport)", id_);
}
