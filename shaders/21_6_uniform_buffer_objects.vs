#version 330 core
layout (location = 0) in vec3 aPos;

// layout (std140) = 
// We specify the size of objects but we do not specify the spacing between objects
// within the UBO. For instance, a vec3 and a float next to eachother cannot be
// handled by all hardware - and therefore we must pad a vec3 to be an array of 
// 4 floats.
// GLSL, by default, uses a shared layout (i.e. shared between multiple programs).
// With a shared layout, GLSL can reposition variables for optimization as long as
// the order remains intact.
// General practice is to NOT use a shared layout but use layout std140. std140
// explicitly states memory layout for each variable type by standardizing their
// respective offsets. Since it is standardized, we can manually configure.
// 
// Scalars (ints, bools, floats) = Base alignment of N
// Vectors = 2N or 4N (i.e. vec3 has 4N alignment)
// Mats = Array of column vectors with each having column having vec4 alignment
// Struct = Equal to computed size of its elements but padded to vec4.
// 
// With these rules in place, we can fill with glBufferSubData
layout (std140) uniform Matrices
{
    // Projection and view matrices are a perfect use of uniform buffer objects because
    // now we only have to set these matrices once.
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}