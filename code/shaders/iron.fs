#version 330 core

out vec4 frag_color;

in vec2 texcoord;
in vec3 normal;
in vec3 frag_pos;

uniform sampler2D texture1;

uniform vec3 obj_color;
uniform vec3 ambientlight_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
  vec3 norm = normalize(normal);
  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 light_dir = normalize(light_pos - frag_pos);

  // ambient light
  float ambient_strength = 0.3;
  vec3 ambient = ambient_strength*ambientlight_color;

  // diffuse light
  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = diff*ambientlight_color;

  // specular light
  float specular_strength = 0.8;
  vec3 reflect_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
  vec3 specular = specular_strength*spec*ambientlight_color;

  // combine light
  vec3 light = diffuse + ambient + specular;

  frag_color = texture(texture1, texcoord)*vec4(obj_color*light,1.0);
}
