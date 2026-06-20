#include "navbar.h"
#include "src/gui/widgets/widget.h"
#include "src/util/logger.h"
#include "src/assets/asset_manager.h"

Navbar::Navbar(const std::string name) {
	Log::debug("navbar initialized");

	name_ = name;

	active[0] = true;
	for (u32 i = 1; i < ICON_COUNT; i++) active[i] = false;
}

Navbar::~Navbar(void) {

}

void Navbar::draw(void) {

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f,4.0f));
	ImGui::Begin(name_.c_str());


	constexpr f32 padding = 0.0f;
	ImVec2 avail = ImGui::GetContentRegionAvail();
	ImVec2 button_size(avail.x - padding, avail.x - padding);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// draws border
	ImVec2 pos = ImGui::GetWindowPos();
	ImVec2 size = ImGui::GetWindowSize();
	draw_list->AddLine(
		ImVec2(size.x - 1.0f, pos.y),
		ImVec2(size.x - 1.0f, pos.y + size.y),
		IM_COL32(255, 255, 255, 255),
		3.0f
	);

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

	if (Widget::icon_button(AssetManager::get_icon(ICON_ASSET_DASHBOARD).id, "DASHBOARD", button_size, active[0]))
		toggle_nav(0);
	if (Widget::icon_button(AssetManager::get_icon(ICON_ASSET_MAP).id, "MAP", button_size, active[1]))
		toggle_nav(1);
	if (Widget::icon_button(AssetManager::get_icon(ICON_ASSET_PLAN).id, "PLAN", button_size, active[2]))
		toggle_nav(2);
	if (Widget::icon_button(AssetManager::get_icon(ICON_ASSET_DRONE).id, "VEHICLE", button_size, active[3]))
		toggle_nav(3);
	if (Widget::icon_button(AssetManager::get_icon(ICON_ASSET_LOG).id, "LOGS", button_size, active[4]))
		toggle_nav(4);

	ImGui::SetCursorPosY(
		avail.y - button_size.y
	);

	if (Widget::icon_button(AssetManager::get_icon(ICON_ASSET_SETTING).id, "SETTINGS",  button_size, active[5]))
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


