#include "image_loader.h"
#include <lib/stb/stb_image.h>
#include "src/util/logger.h"

#include "src/gui/imgui_wrapper/backend_metal_defines.h"

static Renderer::BackendInitDesc* backend_desc;

void set_backend_desc(Renderer::BackendInitDesc* desc) {
	backend_desc = desc;
}

Texture load_imgui_texture(const char* path) {
    Texture result = { 0, 0, 0 };

    int w, h, channels;
    unsigned char* pixels = stbi_load(path, &w, &h, &channels, 4);
    if (!pixels) {
		Log::error("failed at loading: {}", path);
        return result;
	}

    MTLTextureDescriptor* desc =
        [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                           width:w
                                                          height:h
                                                       mipmapped:NO];
    desc.usage = MTLTextureUsageShaderRead;

    id<MTLTexture> tex = [backend_desc->layer.device newTextureWithDescriptor:desc];
    [tex replaceRegion:MTLRegionMake2D(0, 0, w, h)
           mipmapLevel:0
             withBytes:pixels
           bytesPerRow:4 * w];

    stbi_image_free(pixels);

    result.id =
        (ImTextureID)(__bridge_retained void*)tex;
    result.width  = w;
    result.height = h;

    return result;
}

void unload_texture(Texture& tex) {
    if (tex.id == 0)
        return;

	// ARC release texture
    (void)(__bridge_transfer id<MTLTexture>)(void*)tex.id;

    tex.id     = 0;
    tex.width  = 0;
    tex.height = 0;
}
