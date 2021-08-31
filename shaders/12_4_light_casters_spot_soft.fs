#version 330 core
struct Material {
  sampler2D diffuse;
  sampler2D specular;
  // The scattering/radius of the specular highlight
  float shininess;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutoff;
  float outer_cutoff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 Normal;
in vec3 FragPosition;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform SpotLight light;

vec3 CalculateSoftSpotLight(SpotLight light, vec3 normal,
      vec3 frag_position, vec3 view_direction);
void main()
{
  // Ensure the normal vector is normalized
  vec3 norm = normalize(Normal);

  vec3 view_direction = normalize(viewPosition - FragPosition);
  vec3 result = CalculateSoftSpotLight(light, norm, FragPosition, view_direction);
  FragColor = vec4(result, 1.0);
}

// Soft spot light: To create smooth edges,  we have an inner and outer cone.
// The outer cone gradually dims the light from inner to the edges of the outer
// cone.
//
// The intensity value is 0.0 when either negative or outside the spotlight, higher
// than 1.0 when inside the inner cone, and somewhere  in between around the edges.
vec3 CalculateSoftSpotLight(SpotLight light, vec3 normal,
      vec3 frag_position, vec3 view_direction)
{
  vec3 light_direction = normalize(light.position - frag_position);
  float theta = dot(light_direction, normalize(-light.direction));
  float epsilon = light.cutoff - light.outer_cutoff;
  float intensity = clamp((theta -  light.outer_cutoff) / epsilon, 0.0, 1.0);

  // Ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  // Diffuse shading
  float diff = max(dot(normal, light_direction), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

  // Specular shading
  vec3 reflect_direction = reflect(-light_direction, normal);
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

  // Attenuation
  float dist = length(light.position - frag_position);
  float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  // We'll leave ambient unaffected so we always have a little light
  diffuse *= intensity;
  specular *= intensity;

  return (ambient + diffuse + specular);
}

