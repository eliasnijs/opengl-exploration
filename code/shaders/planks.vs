#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out vec2 texcoord;
out vec3 normal;
out vec3 frag_pos;

uniform mat4 M_model;
uniform mat4 M_view;
uniform mat4 M_projection;

void main()
{
   gl_Position = M_projection*M_view*M_model*vec4(a_pos, 1.0);
   texcoord = a_texcoord;
   normal = a_normal;
   frag_pos = vec3(M_model*vec4(a_pos, 1.0));
}
