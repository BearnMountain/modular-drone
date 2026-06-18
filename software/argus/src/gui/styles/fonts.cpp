#include "fonts.h"
#include "src/util/logger.h"
#include <cmath>
#include <algorithm>

#include <filesystem>

// reference to each loaded font 
static std::vector<std::string> font_paths_;

static i32 current_font_ = -1; // references to font_book_ index
static FontSizes current_font_size_ = UNIQUE_FONT_SIZES; // offset into font_book_ for ImGui font loading
static std::vector<ImFont*> font_book_;
// [0] font
// [1-FontSize::UNIQUE_FONT_SIZES] resized versions of loaded font
// [FontSize::UNIQUE_FONT_SIZES + 1] next font

void FontBook::load_font(const char* file_path) {
    ImGuiIO& io = ImGui::GetIO();

    // font family index
    const i32 font_index = font_paths_.size();
    font_paths_.push_back(file_path);

    // add all sizes (order MUST match enum)
    font_book_.push_back(io.Fonts->AddFontFromFileTTF(file_path, 9.0f));   // 0
    font_book_.push_back(io.Fonts->AddFontFromFileTTF(file_path, 12.0f));  // 1
    font_book_.push_back(io.Fonts->AddFontFromFileTTF(file_path, 14.0f));  // 2
    font_book_.push_back(io.Fonts->AddFontFromFileTTF(file_path, 16.0f));  // 3
    font_book_.push_back(io.Fonts->AddFontFromFileTTF(file_path, 18.0f));  // 4
    font_book_.push_back(io.Fonts->AddFontFromFileTTF(file_path, 20.0f));  // 5

    // first loaded font becomes active
    if (current_font_ == -1) {
        current_font_ = font_index;
        current_font_size_ = FontSizes::FONT_SIZE_20PX;
        ImGui::PushFont(get_font(current_font_size_));
    }

    io.Fonts->Build();
}

const std::vector<std::string>& FontBook::get_font_list(void) {
	return font_paths_;
}

void FontBook::set_font(std::string_view font) {
	for (u32 i = 0; i < font_paths_.size(); i++) {
		if (font_paths_[i] == font) {
			current_font_ = i*UNIQUE_FONT_SIZES;
			return;
		}
	}

	Log::error("font not loaded yet: {}", font);
}

ImFont* FontBook::get_font(FontSizes size) {
	if (size == UNIQUE_FONT_SIZES)  {
		Log::debug("provide correct PX* for correct .ttf alias");
	}
	u32 stride = size;
	current_font_size_ = size;

	return font_book_[current_font_ + stride];
}

FontSizes FontBook::get_target_font(f32 height) {
	if (height <= 0) { 
		Log::error("incorrect font target height, must be a positive real number");
		return UNIQUE_FONT_SIZES;
	}
	f32 font_sizes[] = {
		 9.0f,
		12.0f,
		14.0f,
		16.0f,
		18.0f,
		20.0f,
	};

	i32 index = -1;
	auto it = std::lower_bound(font_sizes, font_sizes + 6, height);

    if (it == font_sizes)
        return FONT_SIZE_9PX;

    if (it == font_sizes + 6)
        return FONT_SIZE_20PX;

    int hi = int(it - font_sizes);
    int lo = hi - 1;

    index = (std::fabs(font_sizes[lo] - height) <= std::fabs(font_sizes[hi] - height))
			   ? lo
			   : hi;	

	return (FontSizes)index;
}
