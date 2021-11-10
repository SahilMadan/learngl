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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 light_direction) {
    // Transform the light-space fragment position in clip-space to normalized device coordinates.
    // For gl_Position, OpenGL automatically does a perspective divide to ensure [-w, w] is in the
    // range [-1, 1]. But we need to do this ourselves for the light space frag position.
    vec3 projection_coordinates = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // The depth map is in the range [0, 1], so do this for the projection_coordinates also.
    projection_coordinates = projection_coordinates * 0.5 + 0.5;

    // Z-coordinate is the depth of this fragment from the light's position.
    float current_depth = projection_coordinates.z;

    // Shadow Acne!
    // Because the shadow map is limited by resolution, multiple fragments sample the same value when far away
    // from the light source. This becomes an issue when the lght source looks at an angle from the surface.
    // Thus, some are "above" the surface, and some are "below" the surface. I.e. some considered to be in
    // the shadow, some not. Why?
    // Well, the shadow map is literally just a texture of depth, so you need to sample from some other
    // neighboring 'ray' (i.e. value), which might be lower or higher, given the angle, to what is actually
    // the surface.
    //
    // To solve, add a shadow bias.
    //
    // We COULD use some constant like 0.005 which would work, BUT the value is highly dependent on the angle
    // between the light source and the surface. If there is a steep angle, we need a larger bias.
    // A maximum bias of 0.05 and a minimum of 0.005. When the normal and light direction are perpendicular (i.e. light
    // is travelling in the same direction as a surface vector), then use the maximum bias.
    float bias = max(0.05 * (1.0 - dot(normal, light_direction)), 0.005);

    // Percentage-Closer Filtering (PCF)!
    // The shadows are quite blocky because our shadow map is pretty small and one texel usually spans more than one fragment.
    // With PCF, we produce softer shadows by sampling more than once from the depth map, with different UV coordinates.
    // We then combine the results.
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcf_depth = texture(shadowMap, projection_coordinates.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Over-sampling!
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustrum. Note that this means anything
    // outside of the light frustrum will not have visible shadows. In games, usually made sure that this only occurs
    // in the distance.
    if (current_depth > 1.0) {
        shadow = 0.0;
    }
    
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

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, light_direction);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}