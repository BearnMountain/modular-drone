#include "infobar.h"
#include <implot/implot.h>
#include <implot/implot_internal.h>

Infobar::Infobar(const std::string name) {
	Log::debug("infobar initialized");

	name_ = name;
}

void Infobar::draw(void) {
	constexpr f32 padding = 4.0f;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));

	ImGui::Begin(name_.c_str());
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

	ImGui::BeginChild("Telemetry", ImVec2(0, telemetry_h), ImGuiChildFlags_Borders);
	draw_telemetry();
	ImGui::EndChild();

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

void Infobar::draw_telemetry(void) {
	// altitude, ground speed, heading, battery, link quality, gps_status
    static float xs1[1001], ys1[1001];
    for (int i = 0; i < 1001; ++i) {
        xs1[i] = i * 0.001f;
        ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
    }
    static double xs2[20], ys2[20];
    for (int i = 0; i < 20; ++i) {
        xs2[i] = i * 1/19.0f;
        ys2[i] = xs2[i] * xs2[i];
    }
    if (ImPlot::BeginPlot("Line Plots")) {
        ImPlot::SetupAxes("x","y");
        ImPlot::PlotLine("f(x)", xs1, ys1, 1001);
        ImPlot::PlotLine("g(x)", xs2, ys2, 20,{
            ImPlotProp_Marker, ImPlotMarker_Circle,
            ImPlotProp_Flags, ImPlotLineFlags_Segments
        });
        ImPlot::EndPlot();
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
