#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
  // The depth buffer is non-linear, which gives us more precision up close at
  // the cost of some precision when objects are far away. We can visualize it
  // linearly by reversing the perspective projection...
  
  // Convert the depth value back to normalized device coordinates
  float ndc = depth * 2.0 - 1.0;

  // Apply inverse transformation to retrieve its linear depth value
  float linear_depth = (2.0 * near * far) / (far + near - ndc * (far - near));

  return linear_depth;
}

void main()
{
  // Divide by far for demonstration
  float depth = LinearizeDepth(gl_FragCoord.z) / far;

  FragColor = vec4(vec3(depth), 1.0);
}
