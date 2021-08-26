#version 330 core
in vec3 Normal;
in vec3 FragPosition;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main() 
{
  // Ensure the normal vector is normalized
  vec3 norm = normalize(Normal);

  // Calculate the direction vector between light position and fragment
  // position. Also ensure it is normalized.
  vec3 light_direction = normalize(lightPosition - FragPosition);

  // Ambient

  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * lightColor;

  // Diffuse

  // Calculate the diffuse impact of the light on the current fragment by
  // taking the dot product (i.e., gives us a view of how perpendicular these
  // vectors are). If negative, angle between both vectors > 90 degrees.
  float diff = max(dot(norm, light_direction), 0.0);
  vec3 diffuse = diff * lightColor;

  // Specular

  
  float specular_strength = 0.5;

  vec3 view_direction = normalize(viewPosition - FragPosition);
  
  // Negate light_direction because we want a vector pointing FROM the light
  // source to the fragment. Then, get the reflection of that vector if
  // reflected along the normal vector.
  vec3 reflect_direction = reflect(-light_direction , norm);

  // Calculate the dot product between view direction and reflect direction.
  // Remember, if they were perpendicular or not facing eachother this would be
  // 0. Then raise it to the power of 32 (the 'shininess' of the surface).
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
  vec3 specular = specular_strength * spec * lightColor;

  // Result

  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0);
}

