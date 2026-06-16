#include "navbar.h"
#include "src/assets/image_loader.h"
#include "src/gui/widgets/widget.h"
#include "src/util/logger.h"

Navbar::Navbar(const std::string name) {
	Log::debug("navbar initialized");

	name_ = name;

	// loading all icons
	icons[0] = load_imgui_texture("res/img/widgets/dashboard_white.png");
	icons[1] = load_imgui_texture("res/img/widgets/map_white.png");
	icons[2] = load_imgui_texture("res/img/widgets/plan_white.png");
	icons[3] = load_imgui_texture("res/img/widgets/drone_white.png");
	icons[4] = load_imgui_texture("res/img/widgets/log_white.png");
	icons[5] = load_imgui_texture("res/img/widgets/setting_white.png");

	active[0] = true;
	for (u32 i = 1; i < ICON_COUNT; i++) active[i] = false;
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

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.5f,2.5f));
	ImGui::Begin(name_.c_str());


	constexpr f32 padding = 0.0f;
	ImVec2 avail = ImGui::GetContentRegionAvail();
	ImVec2 button_size(avail.x - padding, avail.x - padding);

	// center cursor
	ImGui::SetCursorPosX(
		ImGui::GetCursorPosX() + (avail.x - button_size.x) * 0.5f
	);

	auto toggle_nav = [this](u32 j) {
		for (u32 i = 0; i < ICON_COUNT; i++) active[i] = false;
		active[j] = true;
		nav_change(j);
		return true;
	};

	if (Widget::icon_button(icons[0].id, "DASHBOARD", button_size, active[0]))
		toggle_nav(0);
	if (Widget::icon_button(icons[1].id, "MAP", 	  button_size, active[1]))
		toggle_nav(1);
	if (Widget::icon_button(icons[2].id, "PLAN", 	  button_size, active[2]))
		toggle_nav(2);
	if (Widget::icon_button(icons[3].id, "VEHICLE",   button_size, active[3]))
		toggle_nav(3);
	if (Widget::icon_button(icons[4].id, "LOGS", 	  button_size, active[4]))
		toggle_nav(4);

	ImGui::SetCursorPosY(
		avail.y - button_size.y
	);

	if (Widget::icon_button(icons[5].id, "SETTINGS",  button_size, active[5]))
		toggle_nav(5);


	ImGui::PopStyleVar(1);
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

void Navbar::nav_change(u32 identifier) {
	switch (identifier) {
		case 0: Log::debug("Nav now on Dashboard"); break;
		case 1: Log::debug("Nav now on Map"); break;
		case 2: Log::debug("Nav now on Plan"); break;
		case 3: Log::debug("Nav now on Vehicle"); break;
		case 4: Log::debug("Nav now on Logs"); break;
		case 5: Log::debug("Nav now on Settings"); break;
		default: Log::error("Incorrect nav change value"); break;
	}
}


