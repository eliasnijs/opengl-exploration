#version 330 core

layout (location = 0) in vec3 a_pos;
//layout (location = 1) in vec3 a_color;
layout (location = 1) in vec2 a_texcoord;

//out vec3 mycolor;
out vec2 texcoord;

uniform mat4 M_model;
uniform mat4 M_view;
uniform mat4 M_projection;

void main()
{
   gl_Position = M_projection*M_view*M_model*vec4(a_pos, 1.0);
//  mycolor = a_color;
   texcoord = a_texcoord;
}
