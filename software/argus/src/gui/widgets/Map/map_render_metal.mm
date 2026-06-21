#include "map.h"
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#include <simd/simd.h>
#include "src/gui/imgui_wrapper/backend_metal_defines.h"

ImTextureID Map::create_map_texture(void) {
	
	int width = 30;
	int height = 30;
	id<MTLDevice> device = desc_->command_queue.device;

	// 1. Create the offscreen texture
	MTLTextureDescriptor *texDesc = [MTLTextureDescriptor
		texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm
									  width:width
									 height:height
								  mipmapped:NO];
	texDesc.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
	texDesc.storageMode = MTLStorageModePrivate;

	id<MTLTexture> texture = [device newTextureWithDescriptor:texDesc];
	// Retain it — ARC won't keep this alive once the function returns
	// unless you store it somewhere with a strong reference.
	CFRetain((__bridge CFTypeRef)texture);

	// 2. Build the pipeline (ideally cache this instead of rebuilding every call)
	static id<MTLRenderPipelineState> pipeline = nil;
	if (!pipeline) {
		id<MTLLibrary> library = [device newDefaultLibrary];
		MTLRenderPipelineDescriptor *pd = [MTLRenderPipelineDescriptor new];
		pd.vertexFunction = [library newFunctionWithName:@"triangle_vertex"];
		pd.fragmentFunction = [library newFunctionWithName:@"triangle_fragment"];
		pd.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

		NSError *error = nil;
		pipeline = [device newRenderPipelineStateWithDescriptor:pd error:&error];
		if (!pipeline) {
			NSLog(@"Pipeline error: %@", error);
			return (ImTextureID)0;
		}
	}

	// 3. Render the triangle into the texture
	id<MTLCommandBuffer> cmdBuffer = [desc_->command_queue commandBuffer];

	MTLRenderPassDescriptor *pass = [MTLRenderPassDescriptor new];
	pass.colorAttachments[0].texture = texture;
	pass.colorAttachments[0].loadAction = MTLLoadActionClear;
	pass.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 1);
	pass.colorAttachments[0].storeAction = MTLStoreActionStore;

	id<MTLRenderCommandEncoder> encoder =
		[cmdBuffer renderCommandEncoderWithDescriptor:pass];
	[encoder setRenderPipelineState:pipeline];
	[encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
	[encoder endEncoding];

	[cmdBuffer commit];
	[cmdBuffer waitUntilCompleted]; // simplest for a one-off; avoid in a hot per-frame path

	// 4. Return as ImTextureID
	return (ImTextureID)(intptr_t)texture;
}
