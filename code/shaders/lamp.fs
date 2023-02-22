#version 330 core


out vec4 frag_color;

in vec2 texcoords;

uniform sampler2D tex_color;
uniform vec3      diffuse;

void main()
{
  frag_color = texture(tex_color, texcoords) * vec4(diffuse, 1.0);
}
