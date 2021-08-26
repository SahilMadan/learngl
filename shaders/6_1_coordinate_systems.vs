#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

// Positions the model in the world
uniform mat4 model;
// Positions the world compared to camera
uniform mat4 view;
// Sets the projection matrix (orthographic vs perspective)
uniform mat4 projection;

void main()
{
  // Note that we read multiplication from right to left
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
