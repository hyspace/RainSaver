//
//  RainView.m
//  Rain
//
//  Created by hyspace on 12/7/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#import "RainView.h"
#import "rain.h"
#import "CoreFoundation/CFBundle.h"
#include <syslog.h>

@implementation RainView

- (instancetype)initWithFrame:(NSRect)frame isPreview:(BOOL)isPreview
{
  // get screenshot Todo: sometimes got all black screen
  CGImageRef image_ref = CGWindowListCreateImage(CGRectInfinite, kCGWindowListOptionOnScreenAboveWindow, kCGNullWindowID, kCGWindowImageDefault);
  CGDataProviderRef provider = CGImageGetDataProvider(image_ref);
  CFDataRef dataref = CGDataProviderCopyData(provider);
  width = CGImageGetWidth(image_ref);
  height = CGImageGetHeight(image_ref);
  size_t bpp = CGImageGetBitsPerPixel(image_ref) / 8;
  pixels = (unsigned char *)malloc(width * height * bpp);
  memcpy(pixels, CFDataGetBytePtr(dataref), width * height * bpp);
  CFRelease(dataref);
  CGImageRelease(image_ref);
  
  // get absolute resource path for loading glsl files
  resource_path = [[NSBundle bundleForClass:[self class]] resourcePath];
  
  self = [super initWithFrame:frame isPreview:isPreview];
  if (self) {
    
    NSOpenGLPixelFormatAttribute attribs[] =
    {
      NSOpenGLPFAOpenGLProfile,
      NSOpenGLProfileVersion4_1Core,
      NSOpenGLPFAAccelerated,
      NSOpenGLPFADepthSize,
      16,
      0
    };
    
    NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    
    _view = [[NSOpenGLView alloc] initWithFrame:NSZeroRect pixelFormat:format];
    [_view setWantsBestResolutionOpenGLSurface:YES];
    [self addSubview:_view];
  }
  return self;
}

- (void)dealloc
{
  [_view removeFromSuperview];
  rain_clean();
  free(pixels);
}

- (void)setFrameSize:(NSSize)newSize
{
  [_view setFrameSize:newSize];
  [super setFrameSize:newSize];
}

- (void)startAnimation
{
  _stoped = NO;
  [super startAnimation];
}

- (void)stopAnimation
{
  _stoped = YES;
  [super stopAnimation];
}

- (void)animateOneFrame
{
  [[_view openGLContext] makeCurrentContext];
  if(!_inited){
    rain_init(std::string([resource_path cStringUsingEncoding:NSASCIIStringEncoding]), pixels, (unsigned int)width, (unsigned int)height);
    _inited = YES;
  }else if(!_stoped){
    rain_draw();
  }
  return;
}

- (BOOL)hasConfigureSheet
{
  return NO;
}

- (NSWindow*)configureSheet
{
  return nil;
}
@end
