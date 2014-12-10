//
//  heightfield.h
//  Rain
//
//  Created by hyspace on 11/26/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#ifndef __glfw_test__heightfield__
#define __glfw_test__heightfield__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma pack(1)
struct vertex{
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat v;
  GLfloat nx;
  GLfloat ny;
  GLfloat nz;
};

template <class T>
class HeightField {
  ~HeightField(){
    free(texture_buffer);
    free(index_buffer);
  }
public:
  T * vertex_buffer;
  GLuint * index_buffer;
  GLuint xMax, yMax, vertex_length, index_length;
  GLuint step;
  int attr_length = sizeof(vertex) / sizeof(GLfloat);
  GLushort * texture_buffer;
  
  HeightField(GLuint width, GLuint height, GLuint input_dh){
    if(width % input_dh || height % input_dh){
      printf("invalid dh");
      return;
    }else{
      step =  input_dh;
    }
    yMax = height / step;
    xMax = width / step;
    vertex_length = xMax * yMax * attr_length;
    
    vertex_buffer = (T *) malloc(sizeof(T) * vertex_length);
    vertex * v = (vertex *) vertex_buffer;
    for(int i = 0; i< yMax; ++i){
      for(int j = 0; j< xMax; ++j){
        int index = i * xMax + j;
        v[index].x = step * j;
        v[index].y = step * i;
        v[index].z = 100;
        v[index].v = 0;
      }
    }
    for(int i = 0; i< yMax; ++i){
      for(int j = 0; j< xMax; ++j){
        int index = i * xMax + j;
        vertex * x1 = j > 0 ? &v[index - 1] : &v[index];
        vertex * x2 = j < xMax - 1 ? &v[index + 1] : &v[index];
        vertex * y1 = i > 0 ? &v[index - xMax] : &v[index];
        vertex * y2 = i < yMax - 1 ? &v[index + xMax] : &v[index];
        glm::vec3 nx = glm::vec3(x2->x - x1->x, 0, x2->z - x1->z);
        glm::vec3 ny = glm::vec3(0, y2->y - y1->y, y2->z - y1->z);
        glm::vec3 n = glm::normalize(glm::cross(nx, ny));
        
        v[index].nx = n.x;
        v[index].ny = n.y;
        v[index].nz = n.z;
      }
    }
    
    index_length = (xMax + 1) * 2 * (yMax - 1) - 2;
    index_buffer = (GLuint *) malloc(sizeof(GLuint) * index_length);
    int index=0, up=0, j = 0, count = 0;
    for(int i = 0; i < yMax - 1; ++i){
      for(j = 0; j < xMax; ++j){
        index = i * xMax + j;
        up = index + xMax;
        index_buffer[count++] = index;
        index_buffer[count++] = up;
      }
      int up2 = up;
      if(i < j - 2){
        index_buffer[count++] = up2;
        index_buffer[count++] = up2 - xMax + 1;
      }
    }
  };
};

#endif /* defined(__glfw_test__heightfield__) */
