#include "fonts.h"
#include "src/util/logger.h"

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
	current_font_ = font_book_.size();
	font_paths_.push_back(file_path);

	// adds all sizes of the font
	ImGuiIO& io = ImGui::GetIO();
	font_book_.push_back(
		io.Fonts->AddFontFromFileTTF(file_path, 9.0f)
	);
	font_book_.push_back(
		io.Fonts->AddFontFromFileTTF(file_path, 12.0f)
	);
	font_book_.push_back(
		io.Fonts->AddFontFromFileTTF(file_path, 14.0f)
	);
	font_book_.push_back(
		io.Fonts->AddFontFromFileTTF(file_path, 16.0f)
	);
	font_book_.push_back(
		io.Fonts->AddFontFromFileTTF(file_path, 18.0f)
	);
	font_book_.push_back(
		io.Fonts->AddFontFromFileTTF(file_path, 20.0f)
	);

	if (current_font_size_ == FontSizes::UNIQUE_FONT_SIZES) {
		current_font_size_ = FontSizes::FONT_SIZE_20PX;
		ImGui::PushFont(font_book_[current_font_size_]);
	}



	io.Fonts->Build();
}

const std::vector<std::string>& FontBook::get_font_list(void) {
	return font_paths_;
}

void FontBook::set_font(std::string_view font) {
	for (u32 i = 0; i < font_paths_.size(); i += static_cast<u32>(FontSizes::UNIQUE_FONT_SIZES)) {
		if (font_paths_[i] == font) {
			current_font_ = i;
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


