#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    // I is the view direction vector (eye to surface)
    vec3 I = normalize(Position - cameraPos);
    // R is the reflection direction vector (surface to cubemap face)
    // This reflected across the surface normal
    vec3 R = reflect(I, normalize(Normal));
    // Use the reflection vector to sample from the cubemap
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}