#version 330 core

struct Material
{
  sampler2D diffuse;
  vec3  specular;
  float shineness;
};

struct Light
{
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

out vec4 frag_color;

in vec3 frag_pos;
in vec3 normal;
in vec2 texcoords;

uniform vec3     view_pos;
uniform Material material;
uniform Light    light;

void main()
{
  // ambient
  vec3 ambient = light.ambient*texture(material.diffuse, texcoords).rgb;

  // diffuse
  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(light.position - frag_pos);
  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = light.diffuse*diff*texture(material.diffuse, texcoords).rgb;


  // TODO(Elias): This is bugged !!!!!!!
  // specular
  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 reflect_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shineness);
  vec3 specular = light.specular*(spec*material.specular);

  // specular
  vec3 result = ambient + diffuse + specular;
  frag_color = vec4(result,1.0);
}
