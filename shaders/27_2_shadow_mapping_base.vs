#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

    // Good description of inverse transpose:
    // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
    // TLDR:
    // We cannot use the same matrix to transpose normals in the case of non-uniform scaling.
    // Previously, we knew the scaling was uniform so we just used the model. We want a
    // matrix with the same rotation, but INVERTED scaling (i.e. if x = 0.5x, then the
    // normal x = 2x).
    // For any number of rotations and scales, we can express as:
    // model3x3 = Rotation1 * Scale * Rotation2
    // model3x3_normal = Rotation1 * Scale^-1 * Rotation2 (i.e. invert the scale)
    // Inverse of any rotation matrix is its transpose (it is orthogonal). 
    // Also note that for pure-scale matrices (along the diagonal), a transpose operation
    // does nothing.
    // So, inverse/transpose will not affect the rotations, but gives us the inverse of the
    // scale matrix.
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    // The frag position in light space
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}