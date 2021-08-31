#version 330 core
struct Material {
  sampler2D diffuse;
  sampler2D specular;
  // The scattering/radius of the specular highlight
  float shininess;
};

struct PointLight {
  vec3 position;

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
uniform PointLight light;

vec3 CalculatePointLight(PointLight light, vec3 normal,
      vec3 frag_position, vec3 view_direction);

void main()
{
  // Ensure the normal vector is normalized
  vec3 norm = normalize(Normal);

  vec3 view_direction = normalize(viewPosition - FragPosition);
  vec3 result = CalculatePointLight(light, norm, FragPosition, view_direction);
  FragColor = vec4(result, 1.0);
}

// Point light: Light source with a given position that illuminates in all directions.
// Light fades over a distance.  For example, a torch or a light-bulb.
//
// Attenuation: Reduce intensity of light over a certain distance. Attenuation eqn
// (better than linear eqn which looks fake):
// F_att  = 1.0 / (K_c + K_l * d + K_q * d^2)
// 
// K_c = Usually kept at 1.0 so that denominator doesn't get smaller than 1. Otherwise,
// intensity is boosted at smaller distances.
// 
// K_l = Linearly fades out light for a given distance
// 
// K_q = Light is intense at close range, fades out quickly, then loses brightness at
// slower pace over larger distances.
//
// For setting values, see:
// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
vec3 CalculatePointLight(PointLight light, vec3 normal,
      vec3 frag_position, vec3 view_direction)
{
  vec3 light_direction = normalize(light.position - frag_position);

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

  return (ambient + diffuse + specular);
}
