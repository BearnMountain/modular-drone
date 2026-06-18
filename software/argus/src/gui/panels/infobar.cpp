#include "infobar.h"
#include "src/gui/widgets/widget.h"
#include <implot/implot.h>
#include <implot/implot_internal.h>

Infobar::Infobar(const std::string name) {
	Log::debug("infobar initialized");

	altitude_.push_back(std::make_pair(1.0f, 2.324f));

	name_ = name;
}

void Infobar::draw(void) {
	constexpr f32 padding = 4.0f;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));

	ImGui::Begin(name_.c_str());
	f32 window_width = ImGui::GetContentRegionAvail().x;
	f32 avail_h = ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 3;
	f32 vehicle_h = avail_h * 0.2f;
	f32 telemetry_h = avail_h * 0.5f;
	f32 system_status_h = avail_h * 0.2f;
	f32 flight_mode_h = avail_h * 0.1f;

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);

	ImGui::BeginChild("Vechile", ImVec2(0, vehicle_h), ImGuiChildFlags_Borders);
	ImGui::Text("test");
	ImGui::EndChild();

	draw_telemetry(window_width, telemetry_h);



	ImGui::BeginChild("System Status", ImVec2(0, system_status_h), ImGuiChildFlags_Borders);
	ImGui::Text("test");
	ImGui::EndChild();

	ImGui::BeginChild("Flight Mode", ImVec2(0, flight_mode_h), ImGuiChildFlags_Borders);
	ImGui::Text("test");
	ImGui::EndChild();


	ImGui::PopStyleVar(3);
	ImGui::End();
}

void draw_vehicle(void) {
	
}

void Infobar::draw_telemetry(u32 win_width, u32 win_height) {
	// altitude, ground speed, heading, battery, link quality, gps_status
	f32 rounding = 12.0f; // Radius of the corners
	ImU32 borderColor = IM_COL32(255, 255, 255, 255); // RGBA Border color
	float borderThickness = 1.0f;

	if (ImGui::BeginTable("RoundedTable", 1, ImGuiTableFlags_BordersInner | ImGuiTableFlags_RowBg, ImVec2(0, win_height))) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// creating the bold header background
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImRect header(pos, ImVec2(pos.x + win_width, pos.y + 45.0f));
		ImGui::ItemSize(header);
		if (!ImGui::ItemAdd(header, 0))
			return;
		drawList->AddRectFilled(header.Min, header.Max, IM_COL32(14, 18, 25, 255), 12.0f, ImDrawFlags_RoundCornersTop);

		ImGui::TableNextRow(ImGuiTableRowFlags_None, 45.0f);
		ImGui::TableNextColumn();
		ImGui::Text(" Telemetry");
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImVec2 start = ImGui::GetCursorPos();
		ImGui::TextUnformatted(" Altitude");
		ImGui::Text(" %.1f m/s", static_cast<double>(altitude_.front().second));

		// ----- RIGHT: graph -----
		float graph_width = 140.0f;   // or dynamic

		// Move cursor to the right, same row
		ImGui::SetCursorPos(ImVec2(
			start.x + ImGui::GetContentRegionAvail().x - graph_width,
			start.y
		));
		
		static float xs1[1001], ys1[1001];
		for (int i = 0; i < 1001; ++i) {
			xs1[i] = i * 0.001f;
			ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
		}

		Widget::simple_graph(0.0f, 1.0f, xs1, ys1, 1001);
	


		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(" Ground Speed");



		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(" Heading");



		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(" Battery");



		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(" Link Quality");



		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(" GPS Status");


		ImGui::EndTable();
		
		// creating the rounded table border
		ImVec2 tableMin = ImGui::GetItemRectMin();
		ImVec2 tableMax = ImGui::GetItemRectMax();
		drawList->AddRect(tableMin, tableMax, borderColor, rounding, ImDrawFlags_None, borderThickness);
	}


}

void draw_system_status(void) {

}

void draw_flight_mode(void) {

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
