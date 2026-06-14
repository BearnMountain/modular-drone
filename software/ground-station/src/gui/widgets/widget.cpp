#include "widget.h"
#include "src/assets/image_loader.h"
#include <implot/implot3d.h>
#include <implot/implot3d_internal.h>
#include "src/util/logger.h"

#define CORNER_RADIUS 6.0f

bool Widget::icon_button(ImTextureID icon, const char* label, ImVec2 size, bool& selected) {
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
		// Format is just icon over hte entire button
		const float padding = 4.0f;

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
		// sizes
		float W = max.x - min.x;
		float H = max.y - min.y;

		// split
		float icon_h = H * 0.65f;

		// --- ICON (centered in top area) ---
		float icon_size = icon_h * 0.6f;

		ImVec2 icon_min(
			min.x + (W - icon_size) * 0.5f,
			min.y + (icon_h - icon_size) * 0.5f
		);

		ImVec2 icon_max(
			icon_min.x + icon_size,
			icon_min.y + icon_size
		);

		draw_list->AddImage(icon, icon_min, icon_max);

		// --- LABEL ---
		float label_top    = min.y + icon_h;
		float label_height = max.y - label_top;
		float label_width  = max.x - min.x;

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







