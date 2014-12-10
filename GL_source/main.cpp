//
//  main.cpp
//  glfw-test
//
//  Created by hyspace on 11/26/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "rain.h"

//#include "heightfield.h"
//#include "update_buffer.h"


GLFWwindow * initWindow (GLFWwindow * window){
  /* Initialize the library */
  if (!glfwInit())
    return NULL;
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(1280, 800, "Water Simulation", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return NULL;
  }
  
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString (GL_VERSION); // version as a string
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);
  
  return window;
}

int main(void)
{
  GLFWwindow * window;
  window = initWindow(window);
  
  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  
  rain_init();
  
  double time = glfwGetTime();
  double last_time = time;
  double dt = 1.0 / 60;
  
//  initCL(vertexbuffer, caustic_texture_id);
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
//    glFinish();
    time = glfwGetTime();
    if (time - last_time > dt){
      last_time = time;
      rain_draw();
//
      glfwSwapBuffers(window);
    }
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}