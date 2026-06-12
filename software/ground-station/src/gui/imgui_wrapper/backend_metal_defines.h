#pragma once

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

namespace Renderer {

struct BackendInitDesc {
    CAMetalLayer*               layer;
    id<MTLCommandQueue>         command_queue;
    MTLRenderPassDescriptor*    render_pass_descriptor;
    id<MTLCommandBuffer>        command_buffer;
    id<MTLRenderCommandEncoder> render_encoder;
    id<CAMetalDrawable>         drawable;
};

}
