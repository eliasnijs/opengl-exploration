#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;

uniform mat4 M_model;
uniform mat4 M_view;
uniform mat4 M_projection;

out vec2 texcoord;

void main()
{
   gl_Position = M_projection*M_view*M_model*vec4(a_pos, 1.0);
   texcoord = a_texcoord;
}
