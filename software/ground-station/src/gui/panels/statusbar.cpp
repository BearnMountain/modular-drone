#include "statusbar.h"

Statusbar::Statusbar(const std::string name) {
	Log::debug("statusbar initialized");

	name_ = name;
}

void Statusbar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("Statusbar");
	ImGui::End();
}

void Statusbar::configure() {
	if (!id_) {
		Log::debug("statusbar configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_))
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	else 
		Log::error("ImGuiID '{}' doesn't exist(Statusbar)", id_);
}
