#version 330 core
// The type of primitive we receive from the vertex shader
// Can be points, lines, triangles, etc.
layout (points) in;

in VS_OUT {
    vec3 color;
} gs_in[];  // NOTE: gs_in not gl_in.

// The primitive type that the geometry shader will output
// Can be points, line_strip, triangle_trip
// Also expected to set maximum number of output vertices.
// If exceeded we won't draw the extra.
layout (triangle_strip, max_vertices = 5) out;

out vec3 fColor;

void main() {
    // Since there is only one input vertex
    fColor = gs_in[0].color;

    // gl_in internally (probably) looks like:
    // in gl_Vertex {
    //     // Vertex we set as vertex shader's output
    //     vec4  gl_Position;
    //     float gl_PointSize;
    //     float gl_ClipDistance[];
    // }
    // Note: gl_in is an array because we receive ALL vertices of a primitive
    // The geometry shader expects us to output AT LEAST one of the primitives
    
    gl_Position = gl_in[0].gl_Position + vec4(-0.2, -0.2, 0.0, 0.0);  // bottom-left
    // Outputs the vector currently set to gl_Position
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.2, -0.2, 0.0, 0.0); // Bottom-right
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.2, 0.2, 0.0, 0.0); // Top-Left
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.2, 0.2, 0.0, 0.0); // Top-Right
    EmitVertex();

    // We can also change the color of the emitted vertex...
    fColor = vec3(1.0, 1.0, 1.0);

    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.4, 0.0, 0.0); // Top
    EmitVertex();

    // Emits vertices of this primitive into the specified output render primitive.
    EndPrimitive();
}