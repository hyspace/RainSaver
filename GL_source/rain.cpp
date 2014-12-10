//
//  rain.cpp
//  Rain
//
//  Created by hyspace on 12/8/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#include "rain.h"

HeightField<GLfloat> * h;

GLfloat dh = 8.0;
GLfloat c = dh * 40;
const double dt = 1.0 / 60;

GLuint vertex_buffer;
GLuint index_buffer;
GLuint vertex_array_id;
GLuint program_id;

GLuint caustic_texture_id;
GLuint sc_texture_id;

int rain_init(std::string res_path, unsigned char * pixels, unsigned int width, unsigned int height){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glFlush();
  
  std::string filePath1, filePath2, filePath3;
  filePath1 = res_path + std::string("/vertex_shader.glsl");
  filePath2 = res_path + std::string("/fragment_shader.glsl");
  filePath3 = res_path + std::string("/t.bmp");
  
  // load shaders
  program_id = LoadShaders( filePath1, filePath2 );
  glUseProgram(program_id);
  
  GLfloat fovy = glm::radians(30.0f);
  GLfloat ratio = (GLfloat) width / (GLfloat) height;
  glm::mat4 projection = glm::perspective(fovy, ratio, 0.1f, 100000.0f);
  glm::vec3 camera_position = glm::vec3(width/2.0, height/2.0, height / 2.0 / tanf(fovy / 2.0));
  glm::mat4 view = glm::lookAt(camera_position, glm::vec3(width/2.0, height/2.0, 0), glm::vec3(0,1,0));
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 mvp = projection * view * model;
  GLuint camera_handler = glGetUniformLocation(program_id, "camera_position");
  glUniform3fv(camera_handler, 1, glm::value_ptr(camera_position));
  GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
  glUniformMatrix4fv(matrix_id, 1, GL_FALSE, glm::value_ptr(mvp));
  
  h = new HeightField<GLfloat> (width,height,(GLuint) dh);
  
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);
  
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * h->vertex_length, h->vertex_buffer, GL_DYNAMIC_DRAW);
  
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * h->index_length, h->index_buffer, GL_DYNAMIC_DRAW);
  
  D_light light;
  light.direction = glm::normalize(glm::vec3(0.0,0.2,1.0));
  light.intensities = glm::vec3(0.98,0.9,0.6);
  GLuint light_direction_handler = glGetUniformLocation(program_id, "light.direction");
  glUniform3fv(light_direction_handler, 1, glm::value_ptr(light.direction));
  GLuint light_intensities_handler = glGetUniformLocation(program_id, "light.intensities");
  glUniform3fv(light_intensities_handler, 1, glm::value_ptr(light.intensities));
  
  GLuint vertex_handler = glGetAttribLocation(program_id, "vertex_position");
  glEnableVertexAttribArray(vertex_handler);
  glVertexAttribPointer(
                        vertex_handler,     // attribute. No partgcicular reason for 0, but must match the layout in the shader.
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        sizeof(vertex), NULL);
  GLuint normal_handler = glGetAttribLocation(program_id, "vertex_normal");
  glEnableVertexAttribArray(normal_handler);
  glVertexAttribPointer(
                        normal_handler,     // attribute. No partgcicular reason for 0, but must match the layout in the shader.
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_TRUE,            // normalized?
                        sizeof(vertex), (const GLvoid*)(4 * sizeof(GLfloat)));
  
  glm::vec2 size = glm::vec2(width, height);
  GLuint size_handler = glGetUniformLocation(program_id, "size");
  glUniform2fv(size_handler, 1, glm::value_ptr(size));
  
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &sc_texture_id);
  glBindTexture(GL_TEXTURE_2D, sc_texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  GLuint sc_texture_handler = glGetUniformLocation(program_id, "sc_texture");
  glUniform1i(sc_texture_handler, 0);

  
  glActiveTexture(GL_TEXTURE1);
  glGenTextures(1, &caustic_texture_id);
  glBindTexture(GL_TEXTURE_2D, caustic_texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, h->xMax, h->yMax, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
  glGenerateMipmap(GL_TEXTURE_2D);
  GLuint caustic_texture_handler = glGetUniformLocation(program_id, "caustic_texture");
  glUniform1i(caustic_texture_handler, 1);
  
  initCL(vertex_buffer, caustic_texture_id);
  return 0;
}

int rain_draw(){
  recompute(h->xMax, h->yMax, dh, dt, c);
  
  glClear(GL_COLOR_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, caustic_texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glDrawElements(
                 GL_TRIANGLE_STRIP,  // mode
                 h->index_length,    // count
                 GL_UNSIGNED_INT,    // type
                 (void*)0            // element array buffer offset
                 );
  glFinish();
  return 0;
}

int rain_clean(){
  freeCL();
  glDisableVertexAttribArray(0);
  // Cleanup
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteBuffers(1, &index_buffer);
  glDeleteTextures(1, &caustic_texture_id);
  glDeleteTextures(1, &sc_texture_id);
  glDeleteVertexArrays(1, &vertex_array_id);
  
  glDeleteProgram(program_id);
  
  free(h);
  return 0;
}