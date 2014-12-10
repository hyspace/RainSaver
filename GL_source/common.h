//
//  common.h
//  Rain
//
//  Created by hyspace on 11/26/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#ifndef __glfw_test__common__
#define __glfw_test__common__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>

#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>

GLuint LoadShaders(std::string& ,std::string&);

#endif /* defined(__glfw_test__common__) */
