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

in vec3 Normal;
in vec3 FragPosition;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight light;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);

void main()
{
  // Ensure the normal vector is normalized
  vec3 norm = normalize(Normal);

  vec3 view_direction = normalize(viewPosition - FragPosition);
  vec3 result = CalculateDirectionalLight(light, norm, view_direction);
  FragColor = vec4(result, 1.0);
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
