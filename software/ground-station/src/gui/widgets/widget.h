#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace GuiWidget {
	bool icon_button(ImTextureID icon, const char* label, ImVec2 size, bool selected);
}
