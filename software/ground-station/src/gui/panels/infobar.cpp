#include "infobar.h"
#include "src/assets/image_loader.h"
#include "src/gui/widgets/widget.h"

static ImTextureID icon;

Infobar::Infobar(const std::string name) {
	Log::debug("infobar initialized");

	name_ = name;
	icon = load_imgui_texture("res/img/widgets/settings.png");
}

void Infobar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("Infobar");
	GuiWidget::icon_button(icon, "Dashboard", ImVec2(90,90), true);
	ImGui::End();
}

void Infobar::configure(void) {
	if (!id_) {
		Log::debug("infobar configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_))
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	else 
		Log::error("ImGuiID '{}' doesn't exist(Infobar)", id_);
}
