//
//  update_buffer.cpp
//  Rain
//
//  Created by hyspace on 11/30/14.
//  Copyright (c) 2014 hyspace. All rights reserved.
//

#include "update_buffer.h"

// opencl
#include "update_heightfield_u.cl.h"
#include "update_heightfield_v.cl.h"
#include "update_caustic.cl.h"

dispatch_queue_t queue;
dispatch_semaphore_t cl_gl_semaphore;

GLfloat * cl_vertex_buffer;
cl_image caustic_texture;
GLuint vertex_buffer_id, texture_id;

int setup_compute_devices()
{
  int err;
  size_t returned_size;
  cl_device_type ComputeDeviceType = CL_DEVICE_TYPE_GPU;
  
  printf("Using active OpenGL context...\n");
  
  // Set the sharegroup.
  CGLContextObj kCGLContext = CGLGetCurrentContext();
  CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
  
  gcl_gl_set_sharegroup(kCGLShareGroup);
  
  // Create a dispatch queue.
  queue = gcl_create_dispatch_queue(ComputeDeviceType, NULL);
  if (!queue)
  {
    printf("Error: Failed to create a dispatch queue!\n");
    return EXIT_FAILURE;
  }
  
  // Create a dispatch semaphore.
  cl_gl_semaphore = dispatch_semaphore_create(0);
  if (!cl_gl_semaphore)
  {
    printf("Error: Failed to create a dispatch semaphore!\n");
    return EXIT_FAILURE;
  }
  
  // Get the device ID.
  cl_device_id ComputeDeviceId = gcl_get_device_id_with_dispatch_queue(queue);
  
  // Report the device vendor and device name.
  cl_char vendor_name[1024] = {0};
  cl_char device_name[1024] = {0};
  err = clGetDeviceInfo(ComputeDeviceId, CL_DEVICE_VENDOR,
                        sizeof(vendor_name), vendor_name,
                        &returned_size);
  err|= clGetDeviceInfo(ComputeDeviceId, CL_DEVICE_NAME,
                        sizeof(device_name), device_name,
                        &returned_size);
  if (err != CL_SUCCESS)
  {
    printf("Error: Failed to retrieve device info!\n");
    return EXIT_FAILURE;
  }
  
  printf("Connecting to %s %s...\n", vendor_name, device_name);
  return CL_SUCCESS;
}

int setup_compute_memory()
{
  cl_vertex_buffer = gcl_gl_create_ptr_from_buffer(vertex_buffer_id);
  if (!cl_vertex_buffer)
  {
    printf("Failed to create vertex_buffer!\n");
    return EXIT_FAILURE;
  }
  
  caustic_texture = gcl_gl_create_image_from_texture(GL_TEXTURE_2D, 0, texture_id);
  if (!caustic_texture)
  {
    printf("Failed to create texture_buffer!\n");
    return EXIT_FAILURE;
  }
  
  return CL_SUCCESS;
}

int initCL(GLuint vertex_buffer_id_input, GLuint texture_id_input){
  vertex_buffer_id = vertex_buffer_id_input;
  texture_id = texture_id_input;
  
  setup_compute_devices();
  setup_compute_memory();
  
  srand((unsigned int) time(NULL));
  
  return CL_SUCCESS;
}

int freeCL(){
  gcl_free(cl_vertex_buffer);
  gcl_release_image(caustic_texture);
  dispatch_release(cl_gl_semaphore);
  dispatch_release(queue);
  
  return CL_SUCCESS;
}

int recompute(GLuint xMax, GLuint yMax, GLfloat dh, GLfloat dt, GLfloat c)
{
  cl_ndrange ndrange;
  
  ndrange.work_dim = 2;
  ndrange.global_work_offset[0] = 0;
  ndrange.global_work_offset[1] = 0;
  ndrange.global_work_size[0] = xMax;
  ndrange.global_work_size[1] = yMax;
  ndrange.local_work_size[0] = 8;
  ndrange.local_work_size[1] = 4;
  
  int r = rand() % 10;
  if(r == 0){
    r = rand() % (xMax * yMax);
  }else{
    r = xMax * yMax + 1;
  }
  
  dispatch_sync(queue,
                 ^{
                   update_heightfield_v_kernel(&ndrange,
                                   cl_vertex_buffer,
                                   dh, dt, c);
                   dispatch_semaphore_signal(cl_gl_semaphore);
                 });
  dispatch_sync(queue,
                ^{
                  update_heightfield_u_kernel(&ndrange,
                                            cl_vertex_buffer,
                                            dh,
                                            dt,
                                            r);
                  dispatch_semaphore_signal(cl_gl_semaphore);
                });
  dispatch_sync(queue,
                ^{
                  update_caustic_kernel(&ndrange,
                                        cl_vertex_buffer,
                                        caustic_texture,
                                        dh);
                  dispatch_semaphore_signal(cl_gl_semaphore);
                });
  return 0;
}