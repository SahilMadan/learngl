#version 330 core
struct Material {
  sampler2D diffuse;
  // The color reflected under specular lighting
  vec3 specular;
  // The scattering/radius of the specular highlight
  float shininess;
};

struct Light {
  vec3 position;

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
uniform Light light;

void main()
{
  // Ensure the normal vector is normalized
  vec3 norm = normalize(Normal);

  // Calculate the direction vector between light position and fragment
  // position. Also ensure it is normalized.
  vec3 light_direction = normalize(light.position - FragPosition);

  // Ambient

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  // Diffuse

  float diff = max(dot(norm, light_direction), 0.0);
  vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff
      * light.diffuse;

  // Specular

  vec3 view_direction = normalize(viewPosition - FragPosition);

  // Negate light_direction because we want a vector pointing FROM the light
  // source to the fragment. Then, get the reflection of that vector if
  // reflected along the normal vector.
  vec3 reflect_direction = reflect(-light_direction , norm);

  // Calculate the dot product between view direction and reflect direction.
  // Remember, if they were perpendicular or not facing eachother this would be
  // 0. Then raise it to the power of 32 (the 'shininess' of the surface).
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * material.specular);

  // Result

  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result, 1.0);
}
