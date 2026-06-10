#include "widget.h"
#include "src/assets/image_loader.h"

bool GuiWidget::icon_button(ImTextureID icon, const char* label, ImVec2 size, bool selected) {
	(void)icon; (void)label;
	ImGui::PushID(label);

	ImGui::InvisibleButton("##button", size);
	bool hovered = ImGui::IsItemHovered();
	bool pressed = ImGui::IsItemClicked();

	// predefined stuff
	ImVec2 pos = ImGui::GetCursorScreenPos();
    ImU32 border =
        selected ? IM_COL32(0,170,255,255) :
        hovered  ? IM_COL32(120,120,120,255) :
                   IM_COL32(40,40,40,255);

	// drawing the button with dynamic border
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled( // Background
        pos, pos + size, IM_COL32(10,10,10,220), 6.0f
	);

    draw_list->AddRect( // Border
        pos, pos + size, border, 6.0f, 0, 2.0f
	);

	// creating icon

	ImGui::PopID();

	return pressed;
}
