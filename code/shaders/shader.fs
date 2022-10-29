#version 330 core

out vec4 frag_color;

//in vec3 mycolor;
in vec2 texcoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
  frag_color = mix(texture(texture1, texcoord), texture(texture2, texcoord), 0.0);
}