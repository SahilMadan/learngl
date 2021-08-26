#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(aPos, 1.0);

  // We do all lighting calculations in world space, so we want the vertex
  // position in world space first.
  FragPosition = vec3(model * vec4(aPos, 1.0));

  // All calculations in the fragment shader are done in world space, but we
  // cannot just use the model matrix for normal vectors. It's a vector so
  // we cannot apply translation. Do this by dropping wx, wy, wz by casting to
  // mat3. Second, if the model matrix has non-uniform scaling then the normal
  // vector would not be perpendicular anymore. Instead, we want to use a
  // 'normal matrix' - the transpose of the inverse. NOTE: Done here for example
  // only; really inefficient. Better performed on the CPU.
  Normal = mat3(transpose(inverse(model))) * aNormal;
}
