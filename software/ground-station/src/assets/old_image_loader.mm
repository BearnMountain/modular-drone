// imgui_texture_loader.mm
// Metal backend — must be compiled as Objective-C++ (.mm).
//
// Compile with:
//   clang++ -x objective-c++ imgui_texture_loader.mm \
//           -fmodules -fobjc-arc \
//           -framework Metal -framework Foundation
//
// ARC is required (-fobjc-arc). If your project uses MRC, replace
// CFRetain/CFRelease with explicit [tex retain] / [tex release].

#include "image_loader.h"

// stb_image — define STB_IMAGE_IMPLEMENTATION in exactly one translation unit.
// If another .cpp already defines it, remove the next two lines.
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#import <Metal/Metal.h>
#import <Foundation/Foundation.h>
#include "lib/imgui/imgui_impl_metal.h"

// ---------------------------------------------------------------------------
//  Wire this up from your engine's Metal context.
//  e.g.  id<MTLDevice> g_MetalDevice = MTLCreateSystemDefaultDevice();
// ---------------------------------------------------------------------------
// Definition — assign this before calling load_imgui_texture().
// If g_MetalDevice is already defined in your render wrapper,
// delete this line and restore: extern id<MTLDevice> g_MetalDevice;
id<MTLDevice> g_MetalDevice = nil;

// ---------------------------------------------------------------------------
//  Internal state
// ---------------------------------------------------------------------------
static uint32_t s_last_w = 0;
static uint32_t s_last_h = 0;

void last_texture_size(uint32_t* out_w, uint32_t* out_h) {
    if (out_w) *out_w = s_last_w;
    if (out_h) *out_h = s_last_h;
}

// ---------------------------------------------------------------------------
//  load_texture
//
//  Returns an ImTextureID that is a CFRetained MTLTexture*.
//  ImGui_ImplMetal treats ImTextureID as id<MTLTexture> directly.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  ImTextureID is typedef'd to ImU64 (uint64_t) in this build.
//  We store the raw ObjC pointer as an integer — safe on 64-bit Apple targets
//  because sizeof(id) == sizeof(uint64_t).  We manage lifetime manually with
//  CFRetain / CFRelease so ARC doesn't collect the texture behind our back.
// ---------------------------------------------------------------------------

// Helpers to convert between ImTextureID (uint64_t) and id<MTLTexture>.
static inline ImTextureID   tex_to_id(id<MTLTexture> t) { return (ImTextureID)(uintptr_t)(__bridge void*)t; }
static inline id<MTLTexture> id_to_tex(ImTextureID   i) { return (__bridge id<MTLTexture>)(void*)(uintptr_t)i; }

ImTextureID load_imgui_texture(const char* path) {
    // ── Decode pixels via stb_image ─────────────────────────────────────────
	fprintf(stderr, "[cwd] %s\n", getcwd(nullptr, 0));
    int w, h, ch;
    stbi_uc* pixels = stbi_load(path, &w, &h, &ch, STBI_rgb_alpha);
    if (!pixels) {
        fprintf(stderr, "[load_texture] stbi_load failed: %s\n", path);
        return (ImTextureID)0;
    }

	// check nil
	if (!g_MetalDevice) {
		fprintf(stderr, "[load_texture] g_MetalDevice is nil — call load_imgui_texture after Metal init\n");
		stbi_image_free(pixels);
		return (ImTextureID)0;
	}

    // ── Build MTLTexture descriptor ─────────────────────────────────────────
    MTLTextureDescriptor* desc =
        [MTLTextureDescriptor
            texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                         width:(NSUInteger)w
                                        height:(NSUInteger)h
                                     mipmapped:NO];

    desc.usage = MTLTextureUsageShaderRead;

    // MTLStorageModeManaged  — macOS (discrete or integrated GPU with separate VRAM).
    //                          CPU writes are visible to GPU after -didModifyRange:.
    // MTLStorageModeShared   — iOS / tvOS / Apple Silicon Mac (unified memory).
    //                          replaceRegion copies directly; no explicit sync needed.
#if TARGET_OS_OSX
    desc.storageMode = MTLStorageModeManaged;
#else
    desc.storageMode = MTLStorageModeShared;
#endif

    // ── Allocate and upload ─────────────────────────────────────────────────
    id<MTLTexture> tex = [g_MetalDevice newTextureWithDescriptor:desc];
    if (!tex) {
        fprintf(stderr, "[load_texture] newTextureWithDescriptor failed\n");
        stbi_image_free(pixels);
        return (ImTextureID)0;
    }

    [tex replaceRegion : MTLRegionMake2D(0, 0, (NSUInteger)w, (NSUInteger)h)
           mipmapLevel : 0
             withBytes : pixels
           bytesPerRow : (NSUInteger)w * 4];

    stbi_image_free(pixels);

#if TARGET_OS_OSX
    // Managed textures need an explicit sync on macOS after CPU writes.
    // Issue a blit encoder to push the update to the GPU-side copy.
    // If you have a command buffer in flight, prefer to reuse it:
    //   [blitEncoder synchronizeTexture:tex slice:0 level:0];
    // Here we create a minimal one-shot buffer for simplicity.
    id<MTLCommandQueue> queue = [g_MetalDevice newCommandQueue];
    id<MTLCommandBuffer> cmd  = [queue commandBuffer];
    id<MTLBlitCommandEncoder> blit = [cmd blitCommandEncoder];
    [blit synchronizeTexture:tex slice:0 level:0];
    [blit endEncoding];
    [cmd commit];
    [cmd waitUntilCompleted];
#endif

    s_last_w = (uint32_t)w;
    s_last_h = (uint32_t)h;

    // Retain manually so the texture outlives this ARC scope.
    // Caller must call free_texture() when done.
    CFRetain((__bridge CFTypeRef)tex);

    return tex_to_id(tex);
}

// ---------------------------------------------------------------------------
//  free_texture — balances the CFRetain taken in load_texture.
//  Call only after the GPU has finished reading the texture (end of frame).
// ---------------------------------------------------------------------------
void free_texture(ImTextureID tex_id) {
    if (!tex_id) return;
    id<MTLTexture> tex = id_to_tex(tex_id);
    CFRelease((__bridge CFTypeRef)tex);
}
