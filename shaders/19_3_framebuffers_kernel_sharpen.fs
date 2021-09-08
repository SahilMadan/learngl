#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

float offset = 1.0 / 300.0;

void main()
{
  // NOTE: offsets is a constant value that we can customize to our liking.
  vec2 offsets[9] = vec2[](
      vec2(-offset, offset),  // top-left
      vec2(0.0f, offset),     // top-center
      vec2(offset, offset),   // top-right
      vec2(-offset, 0.0),     // center-left
      vec2(0.0, 0.0),         // center-center
      vec2(offset, 0.0),      // center-right
      vec2(-offset, -offset), // bottom-left
      vec2(0.0, -offset),     // bottom-center
      vec2(offset, -offset)   // bottom-right
      );

  // A sharpen kernel
  float kernel[9] = float[](
    -1, -1, -1,
    -1, 9, -1,
    -1, -1, -1
  );

  vec3 sample_texture[9];
  for (int i = 0; i < 9; i++) {
    sample_texture[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
  }

  vec3 color = vec3(0.0);
  for (int i = 0; i < 9; i++) {
    // Each surrounding pixel's input is multiplied by its corresponding entry
    // in the kernel, and the color is just additive.
    color += sample_texture[i] * kernel[i];
  }

  FragColor = vec4(color, 1.0);
}
