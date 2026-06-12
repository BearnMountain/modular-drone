#include "navbar.h"
#include "src/assets/image_loader.h"
#include "src/gui/widgets/widget.h"

Navbar::Navbar(const std::string name) {
	Log::debug("navbar initialized");

	name_ = name;

	// loading all icons
	icons[0] = load_imgui_texture("res/img/widgets/dashboard.png");
	icons[1] = load_imgui_texture("res/img/widgets/question.png");
	icons[2] = load_imgui_texture("res/img/widgets/log.png");
	icons[3] = load_imgui_texture("res/img/widgets/map.png");
	icons[4] = load_imgui_texture("res/img/widgets/setting.png");
	icons[5] = load_imgui_texture("res/img/widgets/drone.png");
}

Navbar::~Navbar(void) {
	unload_texture(icons[0]);
	unload_texture(icons[1]);
	unload_texture(icons[2]);
	unload_texture(icons[3]);
	unload_texture(icons[4]);
	unload_texture(icons[5]);
}

void Navbar::draw(void) {
	ImGui::Begin(name_.c_str());
	ImVec2 size = ImGui::GetContentRegionAvail(); 
	ImGui::Text("Navbar");
	static bool selected = false;
	Widget::icon_button(icons[0].id, "Dashboard", ImVec2(size.x - 2, size.x - 2), selected);
	ImGui::End();
}

void Navbar::configure() {
	if (!id_) {
		Log::debug("navbar configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_))
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar |
			ImGuiDockNodeFlags_NoResizeFlagsMask_;
	else 
		Log::error("ImGuiID '{}' doesn't exist(Navbar)", id_);
}
