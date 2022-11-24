#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "include/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#define FAST_OBJ_IMPLEMENTATION
#include "include/fast_obj.h"

#include "include/linmath.h"

#include "base/base.c"
#include "base/utils.c"

#include "engine/linmath_extend.h"
#include "engine/shader.h"
#include "engine/shader.c"
#include "engine/camera.h"
#include "engine/camera.c"
#include "engine/textures.c"
#include "engine/perlin.c"

#include "config.h"

/* constants */
real32 delta_time = 0.0f;
real32 last_frame = 0.0f;
bool32 first_mouse = true;
real32 last_x = 640.0f/2;
real32 last_y = 480.0f/2;

Camera camera;
Shader cube_shader;
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

}

int32
main(int32 argc, char *argv[])
{
  /* initialise glfw */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

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
  uint32 texture_cube_color = texture_load("resources/grass.png");
  uint32 texture_cube_specular = texture_load("resources/sand_specular.png");
  uint32 texture_lamp_color = texture_load("resources/lamp.png");

  /* initialise shaders */
  shader_init("shaders/cube.vs", "shaders/cube.fs", &cube_shader);
  shader_use(&cube_shader);
  shader_set_int32(&cube_shader,"material.diffuse", 0);
  shader_set_int32(&cube_shader,"material.specular", 1);
  shader_init("shaders/lamp.vs", "shaders/lamp.fs", &lamp_shader);
  shader_use(&lamp_shader);
  shader_set_int32(&lamp_shader,"tex_color", 2);

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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)(3*sizeof(real32)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)(6*sizeof(real32)));
  glEnableVertexAttribArray(2);

  /* initiliase camera */
  vec3 camera_start_pos = {-1.0f, 0.0f, 3.0f};
  vec3 camera_start_up = {0.0f, 1.0f, 0.0f};
  camera_init(&camera, camera_start_pos, camera_start_up,
              -80, PITCH_STD,
              SPEED_STD, SENSITIVITY_STD, true, ZOOM_STD);

  /* intialise other */
  uint32 side_length = 200;
  vec3 cube_data[side_length*side_length];
  for (uint32 i = 0; i < side_length; ++i)
  {
    for (uint32 j = 0; j < side_length; ++j)
    {
      uint32 index = side_length*j + i;
      cube_data[index][0] = i;
      cube_data[index][1] = (int32)(perlin2d(i, j, 0.015, 2)*50 - 50);
      cube_data[index][2] = j;
    }
  }

  vec3 pointlight_positions[] = {
    {0.7f, 0.2f, 2.0f},
    {2.3f, -3.3f, -4.0f},
    {-4.0f, 2.0f, -12.0f},
    {0.0f, 0.0f, -3.0f}
  };
  vec3 pointlight_diffuse[] = {
    {0.8f, 0.2f, 0.2f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.8f, 0.8f},
    {0.0f, 0.5f, 0.8f}
  };

  vec4 background_color = {136, 177, 255};

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
    glBindTexture(GL_TEXTURE_2D, texture_cube_color);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_cube_specular);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_lamp_color);

    /* object physics properties */
    vec3 lamp_pos = {0.0f, 0.0f, 0.0f};

    /* object material properties */
    real32 material_shininess = 32.0f;

    /* light properties */
    vec3 sun_direction = {-0.2f, -1.0f, -0.3f};
    vec3 sun_ambient   = {0.3f, 0.25f, 0.3f};
    vec3 sun_diffuse   = {0.8f, 0.8f, 0.8f};
    vec3 sun_specular  = {0.5f, 0.5f, 0.5f};

    vec3 pointlight0_ambient    = {0.05f, 0.05f, 0.05f};
    vec3 pointlight0_specular   = {1.0f, 1.0f, 1.0f};
    float pointlight0_constant  = 1.0f;
    float pointlight0_linear    = 0.09f;
    float pointlight0_quadratic = 0.032f;

    vec3 pointlight1_ambient    = {0.05f, 0.05f, 0.05f};
    vec3 pointlight1_specular   = {1.0f, 1.0f, 1.0f};
    float pointlight1_constant  = 1.0f;
    float pointlight1_linear    = 0.09f;
    float pointlight1_quadratic = 0.032f;

    vec3 pointlight2_ambient    = {0.05f, 0.05f, 0.05f};
    vec3 pointlight2_diffuse    = {0.8f, 0.8f, 0.8f};
    vec3 pointlight2_specular   = {1.0f, 1.0f, 1.0f};
    float pointlight2_constant  = 1.0f;
    float pointlight2_linear    = 0.09f;
    float pointlight2_quadratic = 0.032f;

    vec3 pointlight3_ambient    = {0.05f, 0.05f, 0.05f};
    vec3 pointlight3_specular   = {1.0f, 1.0f, 1.0f};
    float pointlight3_constant  = 1.0f;
    float pointlight3_linear    = 0.09f;
    float pointlight3_quadratic = 0.032f;

    vec3 spotlight_position;
    memcpy(spotlight_position, camera.pos, sizeof(vec3));
    vec3 spotlight_direction;
    memcpy(spotlight_direction, camera.front, sizeof(vec3));
    vec3 spotlight_ambient       = {0.0f, 0.0f, 0.0f};
    vec3 spotlight_diffuse       = {1.0f, 1.0f, 1.0f};
    vec3 spotlight_specular      = {1.0f, 1.0f, 1.0f};
    float spotlight_constant     = 1.0f;
    float spotlight_linear       = 0.09f;
    float spotlight_quadratic    = 0.032f;
    float spotlight_cutoff       = cos(deg_to_rad(12.5f));
    float spotlight_outer_cutoff = cos(deg_to_rad(17.5f));


    /* calculate transform matrices */
    mat4x4 M_projection, M_view;
    mat4x4_perspective(M_projection, deg_to_rad(camera.zoom), width_w/height_w, 0.1f, 100.0f);
    camera_get_M_view(&camera, M_view);

    /* load shaders */

    shader_use(&cube_shader);
    /* load cube shader: properties */
    shader_set_vec3(&cube_shader,   "viewpos",            camera.pos);
    shader_set_real32(&cube_shader, "material.shininess", material_shininess);
    /* load cube shader: lighting */
    shader_set_vec3(&cube_shader, "dirlight.direction", sun_direction);
    shader_set_vec3(&cube_shader, "dirlight.ambient",   sun_ambient);
    shader_set_vec3(&cube_shader, "dirlight.diffuse",   sun_diffuse);
    shader_set_vec3(&cube_shader, "dirlight.specular",  sun_specular);

    shader_set_vec3(&cube_shader,   "pointlights[0].position",  pointlight_positions[0]);
    shader_set_vec3(&cube_shader,   "pointlights[0].ambient",   pointlight0_ambient);
    shader_set_vec3(&cube_shader,   "pointlights[0].diffuse",   pointlight_diffuse[0]);
    shader_set_vec3(&cube_shader,   "pointlights[0].specular",  pointlight0_specular);
    shader_set_real32(&cube_shader, "pointlights[0].constant",  pointlight0_constant);
    shader_set_real32(&cube_shader, "pointlights[0].linear",    pointlight0_linear);
    shader_set_real32(&cube_shader, "pointlights[0].quadratic", pointlight0_quadratic);

    shader_set_vec3(&cube_shader,   "pointlights[1].position",  pointlight_positions[1]);
    shader_set_vec3(&cube_shader,   "pointlights[1].ambient",   pointlight1_ambient);
    shader_set_vec3(&cube_shader,   "pointlights[1].diffuse",   pointlight_diffuse[1]);
    shader_set_vec3(&cube_shader,   "pointlights[1].specular",  pointlight1_specular);
    shader_set_real32(&cube_shader, "pointlights[1].constant",  pointlight1_constant);
    shader_set_real32(&cube_shader, "pointlights[1].linear",    pointlight1_linear);
    shader_set_real32(&cube_shader, "pointlights[1].quadratic", pointlight1_quadratic);

    shader_set_vec3(&cube_shader,   "pointlights[2].position",  pointlight_positions[2]);
    shader_set_vec3(&cube_shader,   "pointlights[2].ambient",   pointlight2_ambient);
    shader_set_vec3(&cube_shader,   "pointlights[2].diffuse",   pointlight_diffuse[2]);
    shader_set_vec3(&cube_shader,   "pointlights[2].specular",  pointlight2_specular);
    shader_set_real32(&cube_shader, "pointlights[2].constant",  pointlight2_constant);
    shader_set_real32(&cube_shader, "pointlights[2].linear",    pointlight2_linear);
    shader_set_real32(&cube_shader, "pointlights[2].quadratic", pointlight2_quadratic);

    shader_set_vec3(&cube_shader,   "pointlights[3].position",  pointlight_positions[3]);
    shader_set_vec3(&cube_shader,   "pointlights[3].ambient",   pointlight3_ambient);
    shader_set_vec3(&cube_shader,   "pointlights[3].diffuse",   pointlight_diffuse[3]);
    shader_set_vec3(&cube_shader,   "pointlights[3].specular",  pointlight3_specular);
    shader_set_real32(&cube_shader, "pointlights[3].constant",  pointlight3_constant);
    shader_set_real32(&cube_shader, "pointlights[3].linear",    pointlight3_linear);
    shader_set_real32(&cube_shader, "pointlights[3].quadratic", pointlight3_quadratic);

    shader_set_vec3(&cube_shader,   "spotlight.position",     spotlight_position);
    shader_set_vec3(&cube_shader,   "spotlight.direction",    spotlight_direction);
    shader_set_vec3(&cube_shader,   "spotlight.ambient",      spotlight_ambient);
    shader_set_vec3(&cube_shader,   "spotlight.diffuse",      spotlight_diffuse);
    shader_set_vec3(&cube_shader,   "spotlight.specular",     spotlight_specular);
    shader_set_real32(&cube_shader, "spotlight.constant",     spotlight_constant);
    shader_set_real32(&cube_shader, "spotlight.linear",       spotlight_linear);
    shader_set_real32(&cube_shader, "spotlight.quadratic",    spotlight_quadratic);
    shader_set_real32(&cube_shader, "spotlight.cutoff",       spotlight_cutoff);
    shader_set_real32(&cube_shader, "spotlight.outer_cutoff", spotlight_outer_cutoff);

    /* load cube shader: transform matrices */
    shader_set_mat4x4(&cube_shader, "M_projection",       M_projection);
    shader_set_mat4x4(&cube_shader, "M_view",             M_view);

    shader_use(&lamp_shader);
    shader_set_mat4x4(&lamp_shader, "M_projection", M_projection);
    shader_set_mat4x4(&lamp_shader, "M_view",       M_view);

    /* render objects */
    shader_use(&cube_shader);
    for (uint32 i = 0; i < ArrayLength(cube_data); ++i)
    {

      mat4x4 M_model;
      mat4x4_translate(M_model, cube_data[i][0], cube_data[i][1], cube_data[i][2]);
      /* mat4x4_rotate(M_model, M_model, 1.0f, 0.3, 0.5, deg_to_rad(20.0f * glfwGetTime())); */
      mat4x4_scale_aniso(M_model, M_model, 1.0f, 1.0f, 1.0f);

      shader_set_mat4x4(&cube_shader, "M_model", M_model);

      glBindVertexArray(cube_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    shader_use(&lamp_shader);
    for (uint32 i = 0; i < ArrayLength(pointlight_positions); ++i)
    {
      mat4x4 M_model;
      mat4x4_translate(M_model, pointlight_positions[i][0], pointlight_positions[i][1], pointlight_positions[i][2]);
      mat4x4_scale_aniso(M_model, M_model, 0.2f, 0.2f, 0.2f);

      shader_set_vec3(&lamp_shader, "diffuse", pointlight_diffuse[i]);
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

  glfwTerminate();

  return 0;
}
