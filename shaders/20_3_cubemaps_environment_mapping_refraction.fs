#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    // Material - Refractive Index
    // Air - 1.00
    // Water - 1.33
    // Ice - 1.309
    // Glass 1.52
    // Diamond 2.42

    // In this case, the ray goes from air to glass, which gives us this ratio.
    float ratio = 1.00 / 1.52;

    // I is the view direction vector (eye to surface)
    vec3 I = normalize(Position - cameraPos);
    // R is the refraction direction vector (surface to cubemap face through
    // the surface).
    vec3 R = refract(I, normalize(Normal), ratio);
    // Use the reflection vector to sample from the cubemap
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}