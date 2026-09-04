#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MD2RendererBridge : NSObject

+ (void)surfaceCreated;
+ (void)drawFrame;
+ (void)setScreenSizeWithWidth:(int)width height:(int)height;
+ (void)setRotationAnglesWithX:(float)x y:(float)y;
+ (void)setScale:(float)scale;
+ (void)setAssetPath:(NSString *)path;

@end

NS_ASSUME_NONNULL_END
