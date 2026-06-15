#pragma once

#include "src/util/math.h"
#include <vector>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Widget {
	// buttons/interactives

	// @param return true if toggeled
	bool icon_button(ImTextureID icon, const char* label, ImVec2 size, f32 padding, bool& selected);

	// plotting data
	void plot_point_cloud(const f32* xn, const f32* yn, const f32* zn, u32 count);
	void simple_graph(u32 min, u32 max, const f32* xn, const f32* yn, u32 count);
}
