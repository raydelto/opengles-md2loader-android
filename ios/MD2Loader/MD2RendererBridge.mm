#import "MD2RendererBridge.h"
#include "Renderer.h"
#include <memory>

static std::unique_ptr<Raydelto::MD2Loader::Renderer> pRenderer;

@implementation MD2RendererBridge

+ (void)initialize {
    if (self == [MD2RendererBridge class]) {
        pRenderer = std::make_unique<Raydelto::MD2Loader::Renderer>();
    }
}

+ (void)surfaceCreated {
    if (pRenderer) {
        pRenderer->OnSurfaceCreated();
    }
}

+ (void)drawFrame {
    if (pRenderer) {
        pRenderer->OnDrawFrame();
    }
}

+ (void)setScreenSizeWithWidth:(int)width height:(int)height {
    if (pRenderer) {
        pRenderer->SetScreenSize(width, height);
    }
}

+ (void)setRotationAnglesWithX:(float)x y:(float)y {
    if (pRenderer) {
        pRenderer->SetRotationAngles(x, y);
    }
}

+ (void)setScale:(float)scale {
    if (pRenderer) {
        pRenderer->SetScale(scale);
    }
}

+ (void)setAssetPath:(NSString *)path {
    if (pRenderer) {
        pRenderer->SetAssetPath(std::string([path UTF8String]));
    }
}

@end
