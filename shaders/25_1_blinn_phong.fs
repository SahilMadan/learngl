#version 330 core
in VS_OUT {
  vec3 FragPosition;
  vec3 Normal;
  vec2 TexCoords;
} fs_in;
out vec4 FragColor;

uniform sampler2D floor_texture;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform int blinn;
uniform float specularExponent;

void main() 
{
  // Ensure the normal vector is normalized
  vec3 norm = normalize(fs_in.Normal);

  // Calculate the direction vector between light position and fragment
  // position. Also ensure it is normalized.
  vec3 light_direction = normalize(lightPosition - fs_in.FragPosition);

  // Calculate the color at this particular fragment
  vec3 color = texture(floor_texture, fs_in.TexCoords).rgb;

  // Ambient

  float ambient_strength = 0.05;
  vec3 ambient = ambient_strength * color;

  // Diffuse

  // Calculate the diffuse impact of the light on the current fragment by
  // taking the dot product (i.e., gives us a view of how perpendicular these
  // vectors are). If negative, angle between both vectors > 90 degrees.
  float diff = max(dot(norm, light_direction), 0.0);
  vec3 diffuse = diff * color;

  // Specular

  vec3 view_direction = normalize(viewPosition - fs_in.FragPosition);

  // NOTE: Though we're using the same shader exponent, the effect they have is meant to be different.
  // I.e. you should adjust the exponent to try and get the same result. We only use the same value 
  // across both just for comparison sake only.

  float spec = 0.0;
  if (blinn != 0) {
    // The issue with phong is when the angle between the view direction and the reflection
    // direction is greater than 90 degrees, the resulting dot product becomes negative and
    // results in a specular component of 0. This causes light to be immediately cut off.
    // This is generally not a problem since the view direction is far from the reflection
    // direction, but with a low specular exponent the specular radius is large enough to have
    // a contribution under these conditions.

    // Halway vector between view direction and light direction.
    vec3 halfway_direction = normalize(light_direction + view_direction);

    // The closer the halfway vector aligns with the normal, the higher the specular contribution.
    // When the view direction is now perfectly aligned with the (now imaginary) reflection
    // vector, the halfway vector perfectly aligns with the normal vector (i.e. strong specular).
    // NOW, no matter what angle, the angle between the halfway and normal never exceeds 90.
    spec = pow(max(dot(norm, halfway_direction), 0.0), specularExponent);
  } else {
    // Negate light_direction because we want a vector pointing FROM the light
    // source to the fragment. Then, get the reflection of that vector if
    // reflected along the normal vector.
    vec3 reflect_direction = reflect(-light_direction , norm);

    // Calculate the dot product between view direction and reflect direction.
    // Remember, if they were perpendicular or not facing eachother this would be
    // 0. Then raise it to the power of 32 (the 'shininess' of the surface).
    spec = pow(max(dot(view_direction, reflect_direction), 0.0), specularExponent);
  }

  // Assume bright white light color.
  vec3 specular = vec3(0.3) * spec;

  // Result
  FragColor = vec4(ambient + diffuse + specular, 1.0);
}

