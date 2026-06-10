#pragma once
#include <imgui/imgui.h>
#include <cstdint>
#include "src/util/defines.h"
#include "src/util/logger.h"

// universal metal device defined by imgui_wrapper 
#ifdef __OBJC__
#import <Metal/Metal.h>
extern id<MTLDevice> g_MetalDevice;
#endif

ImTextureID load_imgui_texture(const char* path, u32* width, u32* height);
void unload_texture(ImTextureID id); // polymorphic unloading
