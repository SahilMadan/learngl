#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// As programs get more complicated, you may want to pass several matching in/out
// variables. Interface blocks allow us to group variables together (like struct).
out VS_OUT
{
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vs_out.TexCoords = aTexCoords;
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}