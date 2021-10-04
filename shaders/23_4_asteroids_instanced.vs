#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
// NOTE: The maximum amount allowed for a vertex attribute is a vec4.
// Because mat4 are basically 4 vec4s, we have to reserve 4 vertex attributes
// for this specific matrix - 3, 4, 5, and 6.
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aTexCoords;
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0f);
}