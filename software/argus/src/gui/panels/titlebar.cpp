#include "titlebar.h" 
#include "src/assets/asset_manager.h"
#include "src/gui/styles/fonts.h"

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

	ImGui::Begin(name_.c_str());

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 size = ImGui::GetWindowSize();
	
	f32 padding = size.y * 0.20f;
	/*
Logo | App_Name | Mission | Status | ... | Link % | GNSS | Storage | Time | Alert | Profile
	*/

	// f32 left_width  = 0.22f * size.x;
	//    f32 mid_width   = 0.38f * size.x;
	//    f32 right_width = 0.40f * size.x;

	// cords for all items
	f32 app_name_x = ( 100.0f / 1600.0f) * size.x;
	f32 mission_x  = ( 250.0f / 1600.0f) * size.x;
	f32 status_x   = ( 410.0f / 1600.0f) * size.x;
	f32 link_x     = (1170.0f / 1600.0f) * size.x;
	f32 gnss_x     = (1265.0f / 1600.0f) * size.x;
	f32 storage_x  = (1340.0f / 1600.0f) * size.x;
	f32 time_x     = (1425.0f / 1600.0f) * size.x;
	f32 alert_x    = (1500.0f / 1600.0f) * size.x;
	f32 profile_x  = (1550.0f / 1600.0f) * size.x;

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
				IM_COL32(255, 255, 255, 255),
				info
			);
		};
	

	// logo
	draw_list->AddImage(
		AssetManager::get_icon(ICON_ASSET_SIGNAL_TOWER).id,
		ImVec2(padding, padding),
		ImVec2(size.y - 2*padding, size.y - 2*padding)
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

	draw_info_header("LATTICE", "MISSION CONTROL",  app_name_x);
	draw_info_header("MISSION", "Operation #1", 	mission_x);
	draw_info_header("STATUS", 	"In Progress", 		status_x);
	draw_info_header("STORAGE", "1.2Gb", 			storage_x);
	draw_info_header("LINK", 	"92%", 				link_x);
	draw_info_header("GNSS", 	"RTX FIX", 			gnss_x);
	draw_info_header("14:32:18","UTC-0", 			time_x);

	// mission 

	// status 
	// storage 
	// link 
	// gnss 
	// time 
	// alert 
	// profile


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
