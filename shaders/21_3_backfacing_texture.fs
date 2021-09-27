#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture1;
uniform sampler2D backTexture;

void main()
{
    if (gl_FrontFacing) {
        FragColor = texture(texture1, TexCoords);
    } else {
        FragColor = texture(backTexture, TexCoords);
    }
}
