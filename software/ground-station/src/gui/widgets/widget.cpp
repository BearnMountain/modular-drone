#include "widget.h"
#include <implot/implot.h>
#include <implot/implot_internal.h>
#include <implot/implot3d.h>
#include <implot/implot3d_internal.h>
#include "src/util/logger.h"

#define CORNER_RADIUS 6.0f

bool Widget::icon_button(ImTextureID icon, const char* label, ImVec2 size, f32 padding, bool& selected) {
	ImGui::PushID(label);

	// reservers clickable region
	ImGui::InvisibleButton("##icon_label", size);

	// config for button itself
	bool hovered = ImGui::IsItemHovered();
	bool pressed = ImGui::IsItemClicked();

	if (pressed) {
		selected = !selected;
	}

	// color
	ImU32 background_color = 
        selected ? IM_COL32(40, 140, 255, 255) :
        hovered  ? IM_COL32(60, 60, 60, 255) :
                   ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
	ImU32 border_color = 
        selected ? IM_COL32(90, 200, 255, 255) :
        hovered  ? IM_COL32(150, 150, 150, 255) :
                   IM_COL32(50, 50, 50, 255);

	// drawing ontop of InvisibleButton
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 min = ImGui::GetItemRectMin(); // bound box of parent widget
	ImVec2 max = ImGui::GetItemRectMax(); // bound box of parent widget

	// background + border
	draw_list->AddRectFilled(
		min, max, background_color, CORNER_RADIUS
	);
	if (selected)
		draw_list->AddRect(
			min, max, border_color, CORNER_RADIUS, ImDrawFlags_RoundCornersAll, 1.0f
		);

	// layout inside button
	if (icon && !label) { // with just icon

		// max icon size inside button
		ImVec2 avail = ImVec2(
			(max.x - min.x) - padding * 2.0f,
			(max.y - min.y) - padding * 2.0f
		);

		// keep icon square
		float icon_size = ImMin(avail.x, avail.y);
		ImVec2 image_size(icon_size, icon_size);

		// center of the button
		ImVec2 center = ImVec2(
			(min.x + max.x) * 0.5f,
			(min.y + max.y) * 0.5f
		);

		ImVec2 image_min = ImVec2(
			center.x - image_size.x * 0.5f,
			center.y - image_size.y * 0.5f
		);

		ImVec2 image_max = ImVec2(
			image_min.x + image_size.x,
			image_min.y + image_size.y
		);

		draw_list->AddImage(
			icon,
			image_min,
			image_max
		);
	} else if (icon && label) {
		float W = (max.x - min.x) - padding * 2.0f;  // subtract padding
		float H = (max.y - min.y) - padding * 2.0f;
		ImVec2 padded_min = ImVec2(min.x + padding, min.y + padding); // offset origin

		float icon_h = H * 0.65f;
		float icon_size = icon_h * 0.6f;
		ImVec2 icon_min(
			padded_min.x + (W - icon_size) * 0.5f,
			padded_min.y + (icon_h - icon_size) * 0.5f
		);
		ImVec2 icon_max(
			icon_min.x + icon_size,
			icon_min.y + icon_size
		);
		draw_list->AddImage(icon, icon_min, icon_max);

		float label_top    = padded_min.y + icon_h;
		float label_height = (max.y - padding) - label_top;  // respect bottom padding
		float label_width  = W;
		float base_font_size = ImGui::GetFontSize();

		// 1) Scale by HEIGHT
		float scale_h = label_height / base_font_size;

		// 2) Scale by WIDTH
		ImVec2 full_text_size = ImGui::CalcTextSize(label);
		float scale_w = label_width / full_text_size.x;

		// 3) Pick smallest scale
		float font_scale = ImMin(scale_h, scale_w);
		font_scale = ImClamp(font_scale, 0.5f, 1.0f);

		float font_size = base_font_size * font_scale;

		// recompute text size with final scale
		ImVec2 text_size = full_text_size * font_scale;

		// centered inside label area
		ImVec2 text_pos(
			min.x + (label_width - text_size.x) * 0.5f,
			label_top + (label_height - text_size.y) * 0.5f
		);

		// clip ONLY the label region
		draw_list->PushClipRect(
			ImVec2(min.x, label_top),
			max,
			true
		);

		draw_list->AddText(
			ImGui::GetFont(),
			font_size,
			text_pos,
			IM_COL32(180,180,180,255),
			label
		);

		draw_list->PopClipRect();
	} else {
		Log::debug("cant create just a button with a label, must have icon or label and icon");
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





