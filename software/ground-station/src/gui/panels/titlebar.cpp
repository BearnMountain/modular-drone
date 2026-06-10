#include "titlebar.h" 

Titlebar::Titlebar(const std::string name) {
	Log::debug("titlebar initialized");

	name_ = name;
}

void Titlebar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("Titlebar");
	ImGui::End();
}

void Titlebar::configure(void) {
	if (!id_) {
		Log::debug("titlebar configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_))
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	else 
		Log::error("ImGuiID '{}' doesn't exist(Titlebar)", id_);
}
