#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // Perspective division is performed after the vertex shader has run,
    // dividing gl_Position's xyz by w. The resulting z component is equal to
    // the depth buffer value of the vertex. Using this information, we can set
    // the z component of the output position equal to w, which results in the
    // z component (and depth buffer) to always equal 1.0 for the skybox.
    // 
    // This optimization means we can render the skybox last and we do not need
    // to waste processing power on wasted fragments.
    gl_Position = pos.xyww;
}