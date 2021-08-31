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

vec3 CalculateSpotLight(SpotLight light, vec3 normal,
      vec3 frag_position, vec3 view_direction);
void main()
{
  // Ensure the normal vector is normalized
  vec3 norm = normalize(Normal);

  vec3 view_direction = normalize(viewPosition - FragPosition);
  vec3 result = CalculateSpotLight(light, norm, FragPosition, view_direction);
  FragColor = vec4(result, 1.0);
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
vec3 CalculateSpotLight(SpotLight light, vec3 normal,
      vec3 frag_position, vec3 view_direction)
{
  vec3 light_direction = normalize(light.position - frag_position);
  float theta = dot(light_direction, normalize(-light.direction));

  if (theta > light.cutoff) {
    // See point-light for implementation details

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

  } else {
    // Otherwise, use ambient lighting so the scene isn't completely dark outside
    // the spotlight
    return light.ambient * vec3(texture(material.diffuse, TexCoords));
  }
}

