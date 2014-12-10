//
//  rain.h
//  Rain
//
//  Created by hyspace on 12/8/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#ifndef __glfw_test__rain__
#define __glfw_test__rain__

#include "common.h"
#include "heightfield.h"
#include "update_buffer.h"
//#include "compute.h"


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct {
  glm::vec3 direction;
  glm::vec3 intensities;
}D_light;

int rain_clean();
int rain_draw();
int rain_init(std::string, unsigned char *, unsigned int, unsigned int);

#endif /* defined(__glfw_test__rain__) */
