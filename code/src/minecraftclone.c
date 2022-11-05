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

#include "base/base.c"
#include "base/utils.c"

#include "engine/linmath_extend.h"
#include "engine/shader.h"
#include "engine/shader.c"
#include "engine/camera.h"
#include "engine/camera.c"
#include "engine/textures.c"

#include "config.h"

/* constants */
real32 delta_time = 0.0f;
real32 last_frame = 0.0f;
bool32 first_mouse = true;
real32 last_x = 640.0f/2;
real32 last_y = 480.0f/2;

Camera camera;
Shader cube_shader;
Shader iron_shader;
Shader lamp_shader;

/* function declarations */
internal void scroll_callback(GLFWwindow *window, real64 xoffset, real64 yoffset);
internal void mouse_callback(GLFWwindow* window, real64 xpos, real64 ypos);
internal void process_input(GLFWwindow *window);
internal void framebuffer_size_callback(GLFWwindow *window, int32 w, int32 h);
int32 main(int32 argc, char *argv[]);

/* function implementations */
internal void
scroll_callback(GLFWwindow *window, real64 xoffset, real64 yoffset)
{
  camera_handle_mouse_zoom_input(&camera, yoffset);
}

internal void
mouse_callback(GLFWwindow* window, real64 xpos, real64 ypos)
{
  if (first_mouse)
  {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }
  camera_handle_mouse_movement_input(&camera, xpos - last_x, last_y - ypos);
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
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_handle_keyboard_input(&camera, FORWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_handle_keyboard_input(&camera, BACKWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_handle_keyboard_input(&camera, LEFT, delta_time);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_handle_keyboard_input(&camera, RIGHT, delta_time);

  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
  {
    shader_die(&cube_shader);
    shader_die(&lamp_shader);
    shader_init("shaders/cube.vs", "shaders/cube.fs", &cube_shader);
    shader_init("shaders/lamp.vs", "shaders/lamp.fs", &lamp_shader);
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
    DebugLogError("failed to create window");
    return -1; /* early return statement */
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  /* load opengl function pointers */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    DebugLogError("failed to initialise GLAD");
    return -1; /* early return statment */
  }

  /* set global opengl state */
  glEnable(GL_DEPTH_TEST);

  /* load textures */
  uint32 texture_cube = texture_load("resources/container2.png");

  /* initialise shaders */
  shader_init("shaders/cube.vs", "shaders/cube.fs", &cube_shader);
  shader_use(&cube_shader);
  shader_set_int32(&cube_shader,"material.diffuse",0);
  shader_init("shaders/lamp.vs", "shaders/lamp.fs", &lamp_shader);

  /* initialise vertex objects */
  #include "models.h"

  uint32 VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

  uint32 cube_VAO;
  glGenVertexArrays(1, &cube_VAO);
  glBindVertexArray(cube_VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)(3*sizeof(real32)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)(6*sizeof(real32)));
  glEnableVertexAttribArray(2);

  uint32 lamp_VAO;
  glGenVertexArrays(1, &lamp_VAO);
  glBindVertexArray(lamp_VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)0);
  glEnableVertexAttribArray(0);

  /* initiliase camera */
  vec3 camera_start_pos = {-3.0f, 1.0f, -3.0f};
  vec3 camera_start_up = {0.0f, 1.0f, 0.0f};
  camera_init(&camera, camera_start_pos, camera_start_up,
              YAW_STD, PITCH_STD,
              SPEED_STD, SENSITIVITY_STD, true, ZOOM_STD);

  /* intialise other */
  vec3 cube_data[] = {
    // pos
    {0.0f, 0.0f, 0.0f},
  };

  vec4 background_color = {10, 10, 10};

  /* main loop */
  while (!glfwWindowShouldClose(window))
  {
    /* time management */
    real32 current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    /* input */
    process_input(window);

    /* render */
    glClearColor(background_color[0]/255, background_color[1]/255,
                 background_color[2]/255, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* bind textures */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_cube);

    /* object properties */
    vec3 material_specular = {0.5f, 0.5f, 0.5f};
    real32 material_shininess = 64.0f;

    vec3 lamp_pos = {1.2f, 1.0f, 2.0f};
    vec3 lamp_ambient = {0.2f, 0.2f, 0.2f};
    vec3 lamp_diffuse = {0.5f, 0.5f, 0.5f};
    vec3 lamp_specular = {1.0f, 1.0f, 1.0f};

    /* calculate transform matrices */
    mat4x4 M_projection, M_view;
    mat4x4_perspective(M_projection, deg_to_rad(camera.zoom), width_w/height_w, 0.1f, 100.0f);
    camera_get_M_view(&camera, M_view);

    /* load shaders */

    shader_use(&cube_shader);
    shader_set_vec3(&cube_shader, "view_pos", camera.pos);
    shader_set_vec3(&cube_shader, "material.specular", material_specular);
    shader_set_real32(&cube_shader, "material.shininess", material_shininess);
    shader_set_vec3(&cube_shader, "light.position", lamp_pos);
    shader_set_vec3(&cube_shader, "light.ambient", lamp_ambient);
    shader_set_vec3(&cube_shader, "light.diffuse", lamp_diffuse);
    shader_set_vec3(&cube_shader, "light.specular", lamp_specular);
    shader_set_mat4x4(&cube_shader, "M_projection", M_projection);
    shader_set_mat4x4(&cube_shader, "M_view", M_view);

    shader_use(&lamp_shader);
    shader_set_mat4x4(&lamp_shader, "M_projection", M_projection);
    shader_set_mat4x4(&lamp_shader, "M_view", M_view);

    /* render objects */

    shader_use(&cube_shader);
    for (uint32 i = 0; i < ArrayLength(cube_data); ++i)
    {
      mat4x4 M_model;
      mat4x4_translate(M_model, cube_data[i][0], cube_data[i][1], cube_data[i][2]);
      mat4x4_scale(M_model, M_model, 1.0f);
      shader_set_mat4x4(&cube_shader, "M_model", M_model);

      glBindVertexArray(cube_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    shader_use(&lamp_shader);
    {
      mat4x4 M_model;
      mat4x4_translate(M_model, lamp_pos[0], lamp_pos[1], lamp_pos[2]);
      mat4x4_scale_aniso(M_model, M_model, 0.2f, 0.2f, 0.2f);
      shader_set_mat4x4(&lamp_shader, "M_model", M_model);

      glBindVertexArray(lamp_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    /* check, call events and swap the buffers */
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &cube_VAO);
  glDeleteVertexArrays(1, &lamp_VAO);
  glDeleteBuffers(1, &VBO);

  shader_die(&lamp_shader);
  shader_die(&cube_shader);
  shader_die(&iron_shader);

  glfwTerminate();

  return 0;
}
