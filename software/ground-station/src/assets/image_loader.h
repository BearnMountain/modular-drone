#pragma once
#include <imgui/imgui.h>
#include "src/gui/imgui_wrapper/render_wrapper.h"
#include "src/util/defines.h"
#include "src/util/logger.h"

struct Texture {
	ImTextureID id;
	u32 width;
	u32 height;
};

void set_backend_desc(Renderer::BackendInitDesc* desc);

Texture load_imgui_texture(const char* path);
void unload_texture(Texture& id); // polymorphic unloading
