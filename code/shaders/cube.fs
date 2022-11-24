#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirLight
{
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight
{
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct SpotLight
{
  vec3 position;
  vec3 direction;
  float cutoff;
  float outer_cutoff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};


out vec4 frag_color;

in vec3 fragpos;
in vec3 normal;
in vec2 texcoords;

uniform vec3     viewpos;
uniform Material material;

uniform DirLight   dirlight;                        // 1 directional ligth
#define NR_OF_POINTLIGHTS 4
uniform PointLight pointlights[NR_OF_POINTLIGHTS];  // 4 pointlights
uniform SpotLight spotlight;                        // 1 spotlight

vec3 calc_dirlight(DirLight light, vec3 normal, vec3 viewdir);
vec3 calc_pointlight(PointLight light, vec3 normal, vec3 viewdir, vec3 fragpos);
vec3 calc_spotlight(SpotLight light, vec3 normal, vec3 viewdir, vec3 fragpos);

vec3
calc_dirlight(DirLight light, vec3 normal, vec3 viewdir)
{
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texcoords));

  vec3  lightdir = normalize(-light.direction);
  float diff     = max(dot(normal, lightdir), 0.0);
  vec3  diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse,  texcoords));

  vec3  reflectdir = reflect(-lightdir, normal);
  float spec       = pow(max(dot(viewdir, reflectdir), 0.0), material.shininess);
  vec3  specular   = light.specular * spec * vec3(texture(material.specular, texcoords));

  return (ambient + diffuse + specular);
}

vec3
calc_pointlight(PointLight light, vec3 normal, vec3 viewdir, vec3 fragpos)
{
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texcoords));

  vec3  lightdir = normalize(light.position - fragpos);
  float diff     = max(dot(normal, lightdir), 0.0);
  vec3  diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse,  texcoords));

  vec3  reflectdir = reflect(-lightdir, normal);
  float spec       = pow(max(dot(viewdir, reflectdir), 0.0), material.shininess);
  vec3  specular   = light.specular * spec * vec3(texture(material.specular, texcoords));

  float distance = length(light.position - fragpos);
  float attenuation =  1.0/(light.constant + light.linear*distance + light.quadratic*(distance*distance));

  return (attenuation*(ambient + diffuse + specular));
}

vec3
calc_spotlight(SpotLight light, vec3 normal, vec3 viewdir, vec3 fragpos)
{
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texcoords));

  vec3  lightdir = normalize(light.position - fragpos);
  float diff     = max(dot(normal, lightdir), 0.0);
  vec3  diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse,  texcoords));

  vec3  reflectdir = reflect(-lightdir, normal);
  float spec       = pow(max(dot(viewdir, reflectdir), 0.0), material.shininess);
  vec3  specular   = light.specular * spec * vec3(texture(material.specular, texcoords));

  float theta = dot(lightdir, normalize(-light.direction));
  float epsilon = light.cutoff - light.outer_cutoff;
  float intensity  = clamp((theta - light.outer_cutoff)/epsilon, 0.0, 1.0);

  float distance = length(light.position - fragpos);
  float attenuation =  1.0/(light.constant + light.linear*distance + light.quadratic*(distance*distance));

  return (intensity*attenuation*(ambient + diffuse + specular));
}

void main()
{
  vec3 result = vec3(0.0);

  vec3 norm = normalize(normal);
  vec3 viewdir = normalize(viewpos - fragpos);

  result += calc_dirlight(dirlight, norm, viewdir);
  for (int i = 0; i < NR_OF_POINTLIGHTS; ++i)
    result += calc_pointlight(pointlights[i], norm, viewdir, fragpos);
  /* result += calc_spotlight(spotlight, norm, viewdir, fragpos); */

  frag_color = vec4(result,1.0);
}
