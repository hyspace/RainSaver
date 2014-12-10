//
//  update_buffer.h
//  Rain
//
//  Created by hyspace on 11/30/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#ifndef __glfw_test__update_buffer__
#define __glfw_test__update_buffer__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>

int initCL(GLuint, GLuint);

int freeCL(void);

int recompute(GLuint, GLuint, GLfloat, GLfloat, GLfloat);

#ifdef __cplusplus
}
#endif

#endif /* defined(__glfw_test__update_buffer__) */
