#include "widget.h"
#include "src/assets/image_loader.h"

void Widget::icon_button(ImTextureID icon, const char* label, ImVec2 size, bool& selected) {
    ImGui::PushID(label);

    // Reserve clickable region
    ImGui::InvisibleButton("##icon_button", size);

    bool hovered = ImGui::IsItemHovered();
    bool pressed = ImGui::IsItemClicked();
	selected = pressed;

    // Item bounds
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();

    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Colors
    ImU32 bg =
        pressed ? IM_COL32(40, 140, 255, 255) :
        hovered  ? IM_COL32(60, 60, 60, 255) :
                   IM_COL32(25, 25, 25, 255);

    ImU32 border =
        selected ? IM_COL32(90, 200, 255, 255) :
        hovered  ? IM_COL32(150, 150, 150, 255) :
                   IM_COL32(50, 50, 50, 255);

    // Draw background + border
    dl->AddRectFilled(min, max, bg, 6.0f);
    dl->AddRect(min, max, border, 6.0f, 0, 2.0f);

    // ----------------------------
    // Layout inside the button
    // ----------------------------

    float padding = 8.0f;

    ImVec2 content_min = ImVec2(min.x + padding, min.y + padding);
    ImVec2 content_max = ImVec2(max.x - padding, max.y - padding);

    float label_height = ImGui::GetTextLineHeight();

    // Icon area (top)
    ImVec2 icon_area_min = content_min;
    ImVec2 icon_area_max = ImVec2(content_max.x, content_max.y - label_height - 4.0f);

    // Center icon
    ImVec2 icon_size = ImVec2(icon_area_max.x - icon_area_min.x,
                              icon_area_max.y - icon_area_min.y);

    ImVec2 icon_pos = ImVec2(
        icon_area_min.x + (icon_size.x * 0.5f),
        icon_area_min.y + (icon_size.y * 0.5f)
    );

    // Draw icon (centered)
    if (icon)
    {
        ImVec2 half = ImVec2(24, 24); // icon size (adjust as needed)

        dl->AddImage(
            icon,
            ImVec2(icon_pos.x - half.x, icon_pos.y - half.y),
            ImVec2(icon_pos.x + half.x, icon_pos.y + half.y)
        );
    }

    // ----------------------------
    // Label (bottom centered)
    // ----------------------------
    ImVec2 text_size = ImGui::CalcTextSize(label);

    ImVec2 text_pos = ImVec2(
        min.x + (size.x - text_size.x) * 0.5f,
        max.y - label_height - 4.0f
    );

    ImU32 text_col =
        selected ? IM_COL32(220, 240, 255, 255) :
        hovered  ? IM_COL32(230, 230, 230, 255) :
                   IM_COL32(180, 180, 180, 255);

    dl->AddText(text_pos, text_col, label);

    ImGui::PopID();
}
