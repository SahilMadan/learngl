#version 330 core
out vec4 FragColor;

// Declared interface block.
in VS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture1;

void main()
{
  FragColor = texture(texture1, fs_in.TexCoords);
}