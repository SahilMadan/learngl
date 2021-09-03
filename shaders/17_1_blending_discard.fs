#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
  // use x, y, z and w
  vec4 tex_color = texture(texture1, TexCoords);
  if (tex_color.a < 0.1) {
    discard;
  }

  FragColor = tex_color;
}
