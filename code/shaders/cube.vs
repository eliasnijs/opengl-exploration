#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoords;

out vec3 fragpos;
out vec3 normal;
out vec2 texcoords;

uniform mat4 M_model;
uniform mat4 M_view;
uniform mat4 M_projection;

void main()
{
   fragpos = vec3(M_model*vec4(a_pos, 1.0));
   normal = mat3(transpose(inverse(M_model)))*a_normal;
   texcoords = a_texcoords;

   gl_Position = M_projection*M_view*vec4(fragpos, 1.0);
}
