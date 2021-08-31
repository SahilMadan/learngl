#version 330 core
struct Material {
  sampler2D diffuse;
  sampler2D specular;
  // The scattering/radius of the specular highlight
  float shininess;
};

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
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
uniform DirectionalLight directionalLight;
#define POINT_LIGHT_COUNT 4
uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform SpotLight spotLight;


vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal,
      vec3 frag_position, vec3 view_direction);
vec3 CalculateSoftSpotLight(SpotLight light, vec3 normal,
      vec3 frag_position, vec3 view_direction);

void main()
{
  // Define an output color value
  vec3 output = vec3(0.0);

  // Properties
  vec3 norm = normalize(Normal);
  vec3 view_direction = normalize(viewPosition - FragPosition);

  // Directional light
  output = CalculateDirectionalLight(directionalLight, norm, view_direction);

  // Point lights
  for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
    output += CalculatePointLight(pointLights[i], norm, FragPosition, view_direction);
  }
  
  // Soft spot light
  output += CalculateSoftSpotLight(spotLight, norm, FragPosition, view_direction);

  FragColor = vec4(output, 1.0);
}

// Directional lights: Great for global lights that illuminate the entire scene
// from a certain direction such that all rays are parallel (such as the sun).
//
// Instead of being given a position of the light, you're directly given the
// direction vector. Note that direction vectors are (x, y, z, 0.0) because we
// do not want the direction vector to be modified by translations. In contrast,
// position vectors for positional lights are (x, y, z, 1.0), since translations
// (such as moving to world space) should have an effect.
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_direction)
{
  vec3 light_direction = normalize(-light.direction);

  // Ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  // Diffuse shading
  float diff = max(dot(normal, light_direction), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

  // Specular shading
  vec3 reflect_direction = reflect(-light_direction, normal);
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

  return (ambient + diffuse + specular);
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

// Flashlight: Spotlight located at the viewer's position and aimed straight
// ahead. At a certain  "angle" we cutoff so that the spotlight is no longer visible.
//
// By "angle", we actually use the cosine(angle). The  reason is that we're
// calculating  the dot product of (light_direction, spotlight_direction) to see if
// they're parallel. The value returned is the cosine (and not an angle), and we can't
// directly compare an angle and a cosine.
// 
// If, instead, we compared angles, we would need the inverse cosine, which is expensive.
//
// Soft spot light: To create smooth edges,  we have an inner and outer cone.
// The outer cone gradually dims the light from inner to the edges of the outer cone.
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
