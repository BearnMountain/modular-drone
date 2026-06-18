#pragma once

#include "src/util/defines.h"
#include <string_view>
#include <imgui/imgui.h>
#include <vector>

enum FontSizes : u8 {
	FONT_SIZE_9PX,
	FONT_SIZE_12PX,
	FONT_SIZE_14PX,
	FONT_SIZE_16PX,
	FONT_SIZE_18PX,
	FONT_SIZE_20PX,
	UNIQUE_FONT_SIZES
}; 

namespace FontBook {

	void load_font(const char* file_path); // Config::font_path
	const std::vector<std::string>& get_font_list(void); // returns all loaded fonts
	void set_font(std::string_view font); // changes current font
	ImFont* get_font(FontSizes size); // get font reference to preloaded sized font

	FontSizes get_target_font(f32 height); // height of area, 0.0f - 1.0f percentage that text fills
};
