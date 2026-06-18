#include "widget.h"
#include <implot/implot.h>
#include <implot/implot_internal.h>
#include <implot/implot3d.h>
#include <implot/implot3d_internal.h>
#include "src/util/logger.h"

#include "src/gui/styles/fonts.h"

#define CORNER_RADIUS 6.0f

bool Widget::icon_button(ImTextureID icon, const char* label, ImVec2 size, bool& selected) {
	if (icon == (ImTextureID)0) {
		Log::debug("must pass an icon for this widget");
		return false;
	}

	if (size.x <= 0.0f || size.y <= 0.0f) {
		Log::debug("must pass valid size");
		return false;
	}

	ImGui::PushID(label);

	// reservers clickable space
	ImGui::InvisibleButton("##button_icon", size);

	// button coloristics depend on pressed or not
	bool pressed = ImGui::IsItemClicked();
	bool hovered = ImGui::IsItemHovered();
	if (pressed) selected = !selected;

	ImU32 background_color = 
        selected ? IM_COL32(4, 8, 13, 255) :
        hovered  ? IM_COL32(60, 60, 60, 255) :
                   ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
	ImU32 border_color = 
        selected ? IM_COL32(14, 51, 93, 255) :
        hovered  ? IM_COL32(150, 150, 150, 255) :
                   IM_COL32(50, 50, 50, 255);


	// drawing
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 max = ImGui::GetItemRectMax();

	// drawing border + background
	draw_list->AddRectFilled(
		min, max, background_color, CORNER_RADIUS
	);
	if (selected)
		draw_list->AddRect(
			min, max, border_color, CORNER_RADIUS, ImDrawFlags_RoundCornersAll, 2.0f
		);

	// internal layout
	if (!label) {
		Log::debug("todo");
	} else {
		// dimensions
		f32 box_height = max.y - min.y;

		f32 percentage_icon = 0.7;

		// icon
		f32 icon_zone_height = percentage_icon * box_height;
		f32 icon_padding = 4.0f; // tweak this
		f32 icon_height = icon_zone_height - icon_padding * 2.0f;

		float icon_x = min.x + (size.x - icon_height) * 0.5f;
		float icon_y = min.y + icon_padding + (icon_zone_height - icon_height) * 0.5f;

		draw_list->AddImage(
			icon,
			ImVec2(icon_x, icon_y),
			ImVec2(icon_x + icon_height, icon_y + icon_height)
		);

		// font
		ImGui::PushFont(FontBook::get_font(FONT_SIZE_9PX));
		ImVec2 text_size = ImGui::CalcTextSize(label);

        f32 label_zone_top = min.y + icon_zone_height;
		f32 label_zone_height = box_height * (1.0f - percentage_icon);
		draw_list->AddText(
			ImVec2(
				min.x + (size.x - text_size.x) * 0.5f,
				label_zone_top + (label_zone_height - text_size.y) * 0.5f
			),
			IM_COL32(255, 255, 255, 255),
			label
		);
		ImGui::PopFont();

	}

	ImGui::PopID();
	return pressed;
}

void Widget::plot_point_cloud(const f32* xn, const f32* yn, const f32* zn, u32 count) {
	if (ImPlot3D::BeginPlot("Scatter Plots")) {
        // ImPlot3D::PlotScatter("Data 1", xn, yn, zn, count);
        ImPlot3DSpec spec;
        spec.Marker = ImPlot3DMarker_Square;
        spec.MarkerSize = 6;
        spec.MarkerLineColor = ImPlot3D::GetColormapColor(1);
        spec.MarkerFillColor = ImPlot3D::GetColormapColor(1);
        spec.FillAlpha = 0.25f;
        ImPlot3D::PlotScatter("Data 2", xn, yn, zn, count, spec);
        ImPlot3D::EndPlot();
    }

}

void Widget::simple_graph(u32 min, u32 max, const f32* xn, const f32* yn, u32 count) {

	ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(2, 2));
	ImPlot::PushStyleColor(ImPlotCol_PlotBg, ImVec4(0,0,0,0));       // transparent bg
	ImPlot::PushStyleColor(ImPlotCol_PlotBorder, ImVec4(0,0,0,0));   // <-- removes inner box
	ImPlotFlags plot_flags = 
		ImPlotFlags_NoTitle     |
		ImPlotFlags_NoLegend    |
		ImPlotFlags_NoMenus     |
		ImPlotFlags_NoBoxSelect |
		ImPlotFlags_NoMouseText |
		ImPlotFlags_NoFrame;
	ImPlotAxisFlags axis_flags = 
		ImPlotAxisFlags_NoLabel | 
		ImPlotAxisFlags_NoDecorations | 
		ImPlotAxisFlags_NoTickMarks | 
		ImPlotAxisFlags_NoTickLabels |
		ImPlotAxisFlags_NoHighlight;

	if (ImPlot::BeginPlot("##miniplot", ImVec2(120, 60), plot_flags)) {
		ImPlot::SetupAxis(ImAxis_X1, nullptr, axis_flags);
		ImPlot::SetupAxis(ImAxis_Y1, nullptr, axis_flags | ImPlotAxisFlags_AutoFit);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1, ImGuiCond_Always);

		// filled area under the line
		ImPlotSpec spec;
		spec.FillColor = ImVec4(0.051f, 0.102f, 0.388f, 1.0f);  // RGBA orange
		spec.FillAlpha = 0.8f;
		// spec.LineColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // green line
		// spec.LineWeight = 2.0f;
		ImPlot::PlotShaded("##fill", xn, yn, count, -INFINITY, spec); // fills in from line to bottom
		ImPlot::PlotLine("##line", xn, yn, count);

		ImPlot::EndPlot();
	}

	// Range label to the right
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("%d", static_cast<i32>(max));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25); // text goes to bottom
	ImGui::Text("%d", static_cast<i32>(min));
	ImGui::EndGroup();

	ImPlot::PopStyleColor(2);
	ImPlot::PopStyleVar(1);
}





