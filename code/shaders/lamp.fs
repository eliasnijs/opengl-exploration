#version 330 core

out vec4 frag_color;

in vec2 texcoord;

uniform sampler2D texture1;

void main()
{
  frag_color = texture(texture1, texcoord)*vec4(1.0);
}
