#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "include/glad.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#include "include/linmath.h"



#include "base.c"
#include "utils.c"

#include "linmath_extend.h"

#include "shader.c"

#include "config.h"

/* constants */


// TODO: abstract camera, mouse and time

/* setup camera */
vec3 camera_pos = {0.0f, 0.0f, 3.0f};
vec3 camera_front = {0.0f, 0.0f, -1.0f};
vec3 camera_up = {0.0f, 1.0f, 0.0f};
real32 camera_speed = 2.5f;
real32 yaw = -90.0f;
real32 pitch = 0.0f;

/* keep track of time */
real32 delta_time = 0.0f;
real32 last_frame = 0.0f;

/* keep track of the mouse */
bool32 first_mouse = true;
real32 mouse_sensitivity = 0.1f;
real32 last_x = 640.0f/2.0f;
real32 last_y = 480.0f/2.0f;

/* function declarations */

internal void framebuffer_size_callback(GLFWwindow *window, int32 w, int32 h);
internal void mouse_callback(GLFWwindow* window, double xpos, double ypos);
internal void process_input(GLFWwindow *window);
int32 main(int32 argc, char *argv[]);

/* function implementations */

internal void
mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (first_mouse)
  {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }


  yaw += (xpos - last_x)*mouse_sensitivity;
  pitch = clamp(-89.0f, pitch + (last_y - ypos)*mouse_sensitivity, 89.0f);

  vec3 direction;
  direction[0] = cos(deg_to_rad(yaw))*cos(deg_to_rad(pitch));
  direction[1] = sin(deg_to_rad(pitch));
  direction[2] = sin(deg_to_rad(yaw))*cos(deg_to_rad(pitch));
  vec3_norm(camera_front, direction);

  last_x = xpos;
  last_y = ypos;
}


internal void
framebuffer_size_callback(GLFWwindow *window, int32 w, int32 h)
{
  width_w = w;
  height_w = h;
  glViewport(0,0,w,h);
}

internal void
process_input(GLFWwindow *window)
{
  /* close the window after q is pressed */
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  real32 camspeed = camera_speed*delta_time;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    vec3 vt;
    vec3_scale(vt, camera_front, camspeed);
    vec3_add(camera_pos, camera_pos, vt);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    vec3 vt;
    vec3_scale(vt, camera_front, camspeed);
    vec3_sub(camera_pos, camera_pos, vt);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    vec3 vt;
    vec3_mul_cross(vt, camera_front, camera_up);
    vec3_norm(vt, vt);
    vec3_scale(vt, vt, camspeed);
    vec3_sub(camera_pos, camera_pos, vt);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    vec3 vt;
    vec3_mul_cross(vt, camera_front, camera_up);
    vec3_norm(vt, vt);
    vec3_scale(vt, vt, camspeed);
    vec3_add(camera_pos, camera_pos, vt);
  }
}

int32
main(int32 argc, char *argv[])
{

  /* initialise glfw */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* create window */
  GLFWwindow *window = glfwCreateWindow(width_w, height_w, program_name, 0, 0);
  if (!window)
  {
    logerr("failed to create window");
    return -1; /* early return statement */
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);


  /* load opengl function pointers */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    logerr("failed to initialise GLAD");
    return -1; /* early return statment */
  }

  /* configure opengl viewport */
  glViewport(0,0,width_w,height_w);


  bool32 success;
  char info_log[512];

  /* set global opengl state */
  glEnable(GL_DEPTH_TEST);

  /* load vertex shader */
  Shader my_shader;
  shader_init("shaders/shader.vs", "shaders/shader.fs", &my_shader);

  /* load texture */
  const char *texture_paths[] = {
    "resources/planks.png",
    "resources/awesomeface.png"
  };
  uint32 textures[arrcnt(texture_paths)];
  int32 width, height, nr_of_channels;
  uint8 *data;

  glGenTextures(1, &textures[0]);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  /* set texture wrapping paramters */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  /* set texture filtereing paramters */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  /* load image */
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(texture_paths[0], &width, &height, &nr_of_channels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    logerr("failed to load texture %s", texture_paths[0]);
  }
  stbi_image_free(data);

  glGenTextures(1, &textures[1]);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  /* set texture wrapping paramters */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  /* set texture filtereing paramters */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  /* load image */
  data = stbi_load(texture_paths[1], &width, &height, &nr_of_channels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    logerr("failed to load texture %s", texture_paths[1]);
  }
  stbi_image_free(data);

  /* load vertex data and setup shader */
  real32 vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };
  vec3 cube_positions[] = {
    { 0.0f,  0.0f,  0.0f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f, -3.5f},
    {-1.7f,  3.0f, -7.5f},
    { 1.3f, -2.0f, -2.5f},
    { 1.5f,  2.0f, -2.5f},
    { 1.5f,  0.2f, -1.5f},
    {-1.3f,  1.0f, -1.5f},
  };

  /* uint32 indices[] = { */
  /*   0,1,3, */
  /*   1,2,3 */
  /* }; */

  uint32 vertex_buffer, vertex_array, elements_buffer;
  glGenVertexArrays(1, &vertex_array);
  glGenBuffers(1, &vertex_buffer);
  /* glGenBuffers(1, &elements_buffer); */

  glBindVertexArray(vertex_array);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements_buffer); */
  /* glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); */

  /* position attribute */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(real32), (void *)0);
  glEnableVertexAttribArray(0);
  /* /1* color attribute *1/ */
  /* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(real32), (void *)(3*sizeof(real32))); */
  /* glEnableVertexAttribArray(1); */
  /* /1* texcoord attribute *1/ */
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(real32), (void *)(3*sizeof(real32)));
  glEnableVertexAttribArray(1);

  shader_use(&my_shader);
  shader_setint32(&my_shader, "texture1", 0);
  shader_setint32(&my_shader, "texture2", 1);

  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

  /* main loop */
  while (!glfwWindowShouldClose(window))
  {
    real32 current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    /* input */
    process_input(window);

    /* render */
    glClearColor(0.0, 0.0, 0.0, 1.0); /* background color */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    mat4x4 M_projection;
    mat4x4_perspective(M_projection, deg_to_rad(45.0f), width_w/height_w, 0.1f, 100.0f);
    shader_setmat4x4(&my_shader, "M_projection", M_projection);

    mat4x4 M_view;
    vec3 camera_target;
    vec3_add(camera_target, camera_pos, camera_front);
    mat4x4_look_at(M_view, camera_pos, camera_target, camera_up);
    shader_setmat4x4(&my_shader, "M_view", M_view);

    shader_use(&my_shader);

    glBindVertexArray(vertex_array);
    for (uint32 i = 0; i < 10; ++i)
    {
      real32 angle = 20.0f * i;

      mat4x4 M_model, M_temp;
      mat4x4_identity(M_model);
      mat4x4_translate(M_model, cube_positions[i][0], cube_positions[i][1], cube_positions[i][2]);
      memcpy(M_temp, M_model, sizeof(mat4x4));
      mat4x4_rotate(M_model, M_temp, 1.0f, 0.3f, 0.5f, deg_to_rad(angle) + 2*glfwGetTime());
      shader_setmat4x4(&my_shader, "M_model", M_model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    /* glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); */

    /* check, call events and swap the buffers */
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  /* clearnup opengl */
  glDeleteVertexArrays(1, &vertex_array);
  glDeleteBuffers(1, &vertex_buffer);
  shader_die(&my_shader);

  /* cleanup glfw */
  glfwTerminate();

  return 0;
}

