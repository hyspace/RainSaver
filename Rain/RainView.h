//
//  RainView.h
//  Rain
//
//  Created by hyspace on 12/7/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#import <ScreenSaver/ScreenSaver.h>

@interface RainView : ScreenSaverView
{
  NSOpenGLView *_view;
  unsigned char *pixels;
  BOOL _stoped;
  BOOL _inited;
  size_t width;
  size_t height;
  NSString * resource_path;
}

@end
