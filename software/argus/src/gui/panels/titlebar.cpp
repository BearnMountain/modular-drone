#include "titlebar.h" 
#include "src/assets/asset_manager.h"
#include "src/gui/styles/fonts.h"
#include "src/util/config.h"

Titlebar::Titlebar(const std::string name) {
	Log::debug("titlebar initialized");

	name_ = name;

}

Titlebar::~Titlebar(void) {
}

/*
Profile:
Max speed / accel limits
RTH behavior
Geofence settings
RC mapping
Sensor weighting (vision vs GNSS)
Logging verbosity
Failsafe thresholds

titlebar:
- logo
- app name
- mission name: "mission x"
- status: "in progress", "return to origin", "initializing", "..."

// other side
- link%
- gnss
- recording size: 'XGb'
- time
- alerts: "low battery", "low link", "high knots"
- profile: above

		*/

void Titlebar::draw(void) {

	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 0, 0, 255));
	ImGui::Begin(name_.c_str());

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 size = ImGui::GetWindowSize();

	// draw entire boarder
	draw_list->AddLine(
		ImVec2(0.0f, size.y),
		ImVec2(size.x, size.y),
		IM_COL32(255, 255, 255, 255),
		5.0f
	);

	
	f32 padding = size.y * 0.20f;

	// cords for all items
	f32 app_name_x = ( 100.0f / 1600.0f) * size.x;
	f32 mission_x  = ( 275.0f / 1600.0f) * size.x;
	f32 status_x   = ( 435.0f / 1600.0f) * size.x;
	f32 link_x     = (1080.0f / 1600.0f) * size.x;
	f32 gnss_x     = (1180.0f / 1600.0f) * size.x;
	f32 storage_x  = (1280.0f / 1600.0f) * size.x;
	f32 time_x     = (1380.0f / 1600.0f) * size.x;
	f32 alert_x    = (1475.0f / 1600.0f) * size.x;
	f32 profile_x  = (1525.0f / 1600.0f) * size.x;

	auto draw_info_header = 
		[&](const char* header, const char* info, f32 x) {
			draw_list->AddText(
				FontBook::get_font(FONT_SIZE_14PX),
				14.0f,
				ImVec2(x, padding),
				IM_COL32(255, 255, 255, 255),
				header
			);
			draw_list->AddText(
				FontBook::get_font(FONT_SIZE_12PX),
				12.0f,
				ImVec2(x, size.y - padding - 14.0f),
				IM_COL32(0x55, 0xff, 0xa8, 255),
				info
			);
		};
	
	

	// logo
	draw_list->AddImage(
		AssetManager::get_icon(ICON_ASSET_SIGNAL_TOWER).id,
		ImVec2(3.0f + padding, padding),
		ImVec2(3.0f + size.y - padding, size.y - padding)
	);

	// app_name 
	// ImGui::PushFont(FontBook::get_font(FONT_SIZE_14PX));
	// f32 font_px_length = ImGui::CalcTextSize("MISSION CONTROL").x;
	// ImGui::PopFont();

	// draw_list->AddLine(
	// 	ImVec2(190.0f, padding), 
	// 	ImVec2(190.0f, size.y - padding), 
	// 	IM_COL32(255, 255, 255, 60)
	// );

	FontBook::set_font(Config::bold_font_path);
	draw_info_header("LATTICE", "MISSION CONTROL",  app_name_x);
	FontBook::set_font(Config::regular_font_path);
	draw_info_header("MISSION", "Operation #1", 	mission_x);
	draw_info_header("STATUS", 	"In Progress", 		status_x);
	draw_info_header("STORAGE", "1.2Gb", 			storage_x);
	draw_info_header("LINK", 	"92%", 				link_x);
	draw_info_header("GNSS", 	"RTX FIX", 			gnss_x);
	draw_info_header("14:32:18","UTC-0", 			time_x);

	draw_list->AddImage(
		AssetManager::get_icon(ICON_ASSET_BELL).id,
		ImVec2(alert_x + padding*1.5f, padding*1.5f),
		ImVec2(alert_x + size.y - padding*1.5f, size.y - padding*1.5f)
	);

	draw_list->AddImage(
		AssetManager::get_icon(ICON_ASSET_USER).id,
		ImVec2(profile_x + 1.5f*padding, 1.5f*padding),
		ImVec2(profile_x + size.y - 1.5f*padding, size.y - 1.5f*padding)
	);

	// mission 

	// status 
	// storage 
	// link 
	// gnss 
	// time 
	// alert 
	// profile

	ImGui::End();
	ImGui::PopStyleColor();
}

void Titlebar::configure(void) {
	if (!id_) {
		Log::debug("titlebar configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_)) {
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	}
	else 
		Log::error("ImGuiID '{}' doesn't exist(Titlebar)", id_);
}
