#pragma once
#include <imgui/imgui.h>
#include <cstdint>
#include "src/util/defines.h"
#include "src/util/logger.h"

struct Texture {
	ImTextureID id;
	u32 width;
	u32 height;
};

ImTextureID load_imgui_texture(const char* path, u32* width, u32* height);
void unload_texture(ImTextureID id); // polymorphic unloading
void cleanup_textures(void); // removes all textures from gpu safely
