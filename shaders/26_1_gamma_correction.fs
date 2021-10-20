#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPosition;
uniform bool gamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPosition, vec3 lightPosition, vec3 lightColor) {
    // Calculate the direction vector between light position and fragment
    // position. Also ensure it is normalized.
    vec3 light_direction = normalize(lightPosition - fragPosition);

    // Diffuse

    // Calculate the diffuse impact of the light on the current fragment by
    // taking the dot product (i.e., gives us a view of how perpendicular these
    // vectors are). If negative, angle between both vectors > 90 degrees.
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular

    vec3 view_direction = normalize(viewPosition - fragPosition);

    // Halway vector between view direction and light direction.
    vec3 halfway_direction = normalize(light_direction + view_direction);

    // The closer the halfway vector aligns with the normal, the higher the specular contribution.
    // When the view direction is now perfectly aligned with the (now imaginary) reflection
    // vector, the halfway vector perfectly aligns with the normal vector (i.e. strong specular).
    // NOW, no matter what angle, the angle between the halfway and normal never exceeds 90.
    float spec = pow(max(dot(normal, halfway_direction), 0.0), 64.0);

    vec3 specular = spec * lightColor;

    // Recall that attenuation is how the light drops off over a distance. Linear equation (1 / distance)
    // gives more believable results when gamma correction is not performed. However, with gamma correction,
    // the linear attenuation looks too weak and the physically correct quadratic equation (1 / distance^2)
    // looks better.
    float dist = length(lightPosition - fragPosition);
    float attenuation = 1.0 / (gamma ? dist * dist : dist);

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}

void main()
{
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    vec3 lighting = vec3(0.0);
    for (int i = 0 ; i < 4; i++) {
        lighting += BlinnPhong(normalize(fs_in.Normal), fs_in.FragPosition, lightPositions[i], lightColors[i]);
    }
    color *= lighting;

    if (gamma) {
        color = pow(color, vec3(1.0 / 2.2));
    }

    FragColor = vec4(color, 1.0);
}

