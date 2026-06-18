#include "toolbar.h"

Toolbar::Toolbar(const std::string name) {
	Log::debug("initialized toolbar");

	name_ = name;
}


void Toolbar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("Toolbar");
	ImGui::End();
}

void Toolbar::configure() {
	if (!id_) {
		Log::debug("viewport configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_))
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	else 
		Log::error("ImGuiID '{}' doesn't exist(Toolbar)", id_);
}
