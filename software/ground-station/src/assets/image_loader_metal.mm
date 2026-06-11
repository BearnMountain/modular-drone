#include "image_loader.h"
#include <lib/stb/stb_image.h>

#import <Metal/Metal.h>

Texture load_imgui_texture(id<MTLDevice> device, const char* path) {
    Texture result = { 0, 0, 0 };

    int w, h, channels;
    unsigned char* pixels = stbi_load(path, &w, &h, &channels, 4);
    if (!pixels)
        return result;

    MTLTextureDescriptor* desc =
        [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                           width:w
                                                          height:h
                                                       mipmapped:NO];
    desc.usage = MTLTextureUsageShaderRead;

    id<MTLTexture> tex = [device newTextureWithDescriptor:desc];
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

    id<MTLTexture> metalTex =
        (__bridge_transfer id<MTLTexture>)(void*)tex.id;

    metalTex = nil; // ARC releases

    tex.id     = 0;
    tex.width  = 0;
    tex.height = 0;
}
