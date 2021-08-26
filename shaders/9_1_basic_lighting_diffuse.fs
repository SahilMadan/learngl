#version 330 core
in vec3 Normal;
in vec3 FragPosition;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

vec3 GetDiffuse();

void main() 
{
  // Ambient Light
  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * lightColor;

  // Diffuse
  vec3 diffuse = GetDiffuse();

  vec3 result = (ambient + diffuse) * objectColor;
  FragColor = vec4(result, 1.0);
}

vec3 GetDiffuse()
{
  // Ensure the normal vector is normalized.
  vec3 norm = normalize(Normal);

  // Calculate the direction vector between light position and fragment
  // position. Also ensure it is normalized.
  vec3 light_direction = normalize(lightPosition - FragPosition);
  
  // Calculate the diffuse impact of the light on the current fragment by
  // taking the dot product (i.e., gives us a view of how perpendicular these
  // vectors are). If negative, angle between both vectors > 90 degrees.
  float diff = max(dot(norm, light_direction), 0.0);
  return diff * lightColor;
}
