#pragma once

#include "src/util/defines.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using TextureID = u32;
using FontID = u32;

constexpr TextureID INVALID_TEXTURE = 0;
constexpr FontID INVALID_FONT = 0;

class AssetManager {
public:
	AssetManager();
	~AssetManager();

	// textures
	TextureID load_texture(std::string_view path);
	// fonts
private:
	// std::vector<...>
}
