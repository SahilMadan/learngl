#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

// See example 15_3_visualize_linear_depth.
float LinearizeDepth(float depth) {
  // Back to normalized device coordinates.
  float z = depth * 2.0 - 1.0;

  return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
  float depth_value = texture(depthMap, TexCoords).r;
  // Perspective projection
  FragColor = vec4(vec3(LinearizeDepth(depth_value) / far_plane), 1.0);

  // Orthographic
  // FragColor = vec4(vec3(depth_value), 1.0);
}