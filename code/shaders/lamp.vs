#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoords;

out vec2 texcoords;

uniform mat4 M_model;
uniform mat4 M_view;
uniform mat4 M_projection;

void main()
{
   vec3 frag_pos = vec3(M_model*vec4(a_pos, 1.0));
   texcoords = a_texcoords;
   gl_Position = M_projection*M_view*vec4(frag_pos, 1.0);
}
