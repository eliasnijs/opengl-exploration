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

#include "config.h"

/* constants */
real32 delta_time = 0.0f;
real32 last_frame = 0.0f;
bool32 first_mouse = true;
real32 last_x = 640.0f/2;
real32 last_y = 480.0f/2;

Camera camera;
Shader planks_shader;
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
    shader_die(&planks_shader);
    shader_die(&lamp_shader);
    shader_init("shaders/planks.vs", "shaders/planks.fs", &planks_shader);
    shader_init("shaders/lamp.vs", "shaders/lamp.fs", &lamp_shader);
    shader_init("shaders/iron.vs", "shaders/iron.fs", &lamp_shader);
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

  /* configure opengl viewport */
  glViewport(0,0,width_w,height_w);

  /* set global opengl state */
  glEnable(GL_DEPTH_TEST);

  /* load textures */
  int32 width, height, nr_of_channels;
  uint8 *data;

  uint32 texture_planks;
  char *texture_planks_path = "resources/planks.png";
  glGenTextures(1, &texture_planks);
  glBindTexture(GL_TEXTURE_2D, texture_planks);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(texture_planks_path, &width, &height, &nr_of_channels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    DebugLogError("failed to load texture %s", texture_planks_path);
  }
  stbi_image_free(data);

  char *texture_lamp_path = "resources/lamp.png";
  uint32 texture_lamp;
  glGenTextures(1, &texture_lamp);
  glBindTexture(GL_TEXTURE_2D, texture_lamp);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(texture_lamp_path, &width, &height, &nr_of_channels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    DebugLogError("failed to load texture %s", texture_lamp_path);
  }
  stbi_image_free(data);

  char *texture_iron_path = "resources/iron.png";
  uint32 texture_iron;
  glGenTextures(1, &texture_iron);
  glBindTexture(GL_TEXTURE_2D, texture_iron);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(texture_iron_path, &width, &height, &nr_of_channels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    DebugLogError("failed to load texture %s", texture_iron_path);
  }
  stbi_image_free(data);

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
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)(3*sizeof(real32)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)(5*sizeof(real32)));
  glEnableVertexAttribArray(2);

  uint32 lamp_VAO;
  glGenVertexArrays(1, &lamp_VAO);
  glBindVertexArray(lamp_VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)(3*sizeof(real32)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(real32), (void *)(5*sizeof(real32)));
  glEnableVertexAttribArray(2);

  /* initialise shaders */
  shader_init("shaders/iron.vs", "shaders/iron.fs", &iron_shader);
  shader_init("shaders/planks.vs", "shaders/planks.fs", &planks_shader);
  shader_init("shaders/lamp.vs", "shaders/lamp.fs", &lamp_shader);

  /* initiliase camera */
  vec3 camera_start_pos = {-3.0f, 0.0f, -3.0f};
  vec3 camera_start_up = {0.0f, 1.0f, 0.0f};
  camera_init(&camera, camera_start_pos, camera_start_up,
              YAW_STD, PITCH_STD,
              SPEED_STD, SENSITIVITY_STD, true, ZOOM_STD);

  /* main loop */
  vec3 cube_positions[] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, 0.0f, 1.0f},
  };



  while (!glfwWindowShouldClose(window))
  {
    /* time management */
    real32 current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    /* inpuut */
    process_input(window);

    /* render */
    glClearColor(40.0f/255, 40.0f/255, 40.0f/255, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_planks);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_lamp);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_iron);

    mat4x4 M_projection, M_view;
    mat4x4_perspective(M_projection, deg_to_rad(camera.zoom), width_w/height_w, 0.1f, 100.0f);
    camera_get_M_view(&camera, M_view);


    vec3 ambientlight_color = {1.0f, 1.0f, 1.0f};
    vec3 light_pos = {
      10.0f * sin(glfwGetTime()),
      0.0f,
      10.0f * cos(glfwGetTime()),
    };

    mat4x4 M_model_lamp;
    mat4x4_identity(M_model_lamp);
    mat4x4_translate(M_model_lamp,  light_pos[0], light_pos[1], light_pos[2]);
    mat4x4_scale(M_model_lamp, M_model_lamp, 0.1f);

    shader_use(&lamp_shader);
    shader_set_int32(&lamp_shader, "texture1", 1);
    shader_set_mat4x4(&lamp_shader, "M_projection", M_projection);
    shader_set_mat4x4(&lamp_shader, "M_view", M_view);
    shader_set_mat4x4(&lamp_shader, "M_model", M_model_lamp);

    glBindVertexArray(lamp_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    for (uint32 i = 0; i < ArrayLength(cube_positions)/2; ++i)
    {
      vec3 obj_color = {1.0f, 1.0f, 1.0f};

      mat4x4 M_model_cube, M_temp_cube;
      mat4x4_identity(M_temp_cube);
      mat4x4_translate(M_temp_cube,  cube_positions[i][0], cube_positions[i][1], cube_positions[i][2]);
      mat4x4_rotate(M_model_cube, M_temp_cube, 0.0f, 1.0f, 0.0f, 0);

      shader_use(&planks_shader);
      shader_set_int32(&planks_shader, "texture1", 0);
      shader_set_vec3(&planks_shader, "obj_color", obj_color);
      shader_set_vec3(&planks_shader, "ambientlight_color", ambientlight_color);
      shader_set_vec3(&planks_shader, "light_pos", light_pos);
      shader_set_vec3(&planks_shader, "view_pos", camera.pos);
      shader_set_mat4x4(&planks_shader, "M_projection", M_projection);
      shader_set_mat4x4(&planks_shader, "M_view", M_view);
      shader_set_mat4x4(&planks_shader, "M_model", M_model_cube);

      glBindVertexArray(cube_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    for (uint32 i = ArrayLength(cube_positions)/2; i < ArrayLength(cube_positions); ++i)
    {
      vec3 obj_color = {1.0f, 1.0f, 1.0f};

      mat4x4 M_model_cube, M_temp_cube;
      mat4x4_identity(M_temp_cube);
      mat4x4_translate(M_temp_cube,  cube_positions[i][0], cube_positions[i][1], cube_positions[i][2]);
      mat4x4_rotate(M_model_cube, M_temp_cube, 0.0f, 1.0f, 0.0f, 0);

      shader_use(&iron_shader);
      shader_set_int32(&iron_shader, "texture1", 2);
      shader_set_vec3(&iron_shader, "obj_color", obj_color);
      shader_set_vec3(&iron_shader, "ambientlight_color", ambientlight_color);
      shader_set_vec3(&iron_shader, "light_pos", light_pos);
      shader_set_vec3(&iron_shader, "view_pos", camera.pos);
      shader_set_mat4x4(&iron_shader, "M_projection", M_projection);
      shader_set_mat4x4(&iron_shader, "M_view", M_view);
      shader_set_mat4x4(&iron_shader, "M_model", M_model_cube);

      glBindVertexArray(cube_VAO);
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
  shader_die(&planks_shader);
  shader_die(&iron_shader);

  glfwTerminate();

  return 0;
}
