#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // Transform the light-space fragment position in clip-space to normalized device coordinates.
    // For gl_Position, OpenGL automatically does a perspective divide to ensure [-w, w] is in the
    // range [-1, 1]. But we need to do this ourselves for the light space frag position.
    vec3 projection_coordinates = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // The depth map is in the range [0, 1], so do this for the projection_coordinates also.
    projection_coordinates = projection_coordinates * 0.5 + 0.5;

    // Closest depth from the light's point of view
    float closest_depth = texture(shadowMap, projection_coordinates.xy).r;

    // Z-coordinate is the depth of this fragment from the light's position.
    float current_depth = projection_coordinates.z;

    float shadow = current_depth > closest_depth ? 1.0 : 0.0;
    
    return shadow;
}

void main() {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 light_color = vec3(1.0);

    // Blinn-Phong

    // Ambient
    vec3 ambient = 0.15 * light_color;
    // Diffuse
    vec3 light_direction = normalize(lightPosition - fs_in.FragPos);
    float diff = max(dot(light_direction, normal), 0.0);
    vec3 diffuse = diff * light_color;
    // Specular
    vec3 view_direction = normalize(viewPosition - fs_in.FragPos);
    float spec = 0.0;
    vec3 halfway_direction = normalize(light_direction + view_direction);
    spec = pow(max(dot(normal, halfway_direction), 0.0), 64.0);
    vec3 specular = spec * light_color;

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}