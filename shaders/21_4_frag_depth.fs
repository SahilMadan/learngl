#version 420 core // Note the GLSL version! From GL 4.2 only.

out vec4 FragColor;
layout (depth_greater) out float gl_FragDepth;

void main()
{
    // glFragCoord is a read-only variable. We can't influence the screen-space coordinates.
    // glFragDepth is an output variable that can be manually set to influence depth value.
    // Using glFragDepth disables early depth testing as soon as we write to it. However,
    // since GL 4.2, we can specify greater/less depth condition, GL can make assumptions
    // about what you'll write to the depth buffer and is still able to do early depth
    // testing.
    FragColor = vec4(1.0);
    // Note: by default, gl_FragDepth = gl_FragCoord.z.
    gl_FragDepth = gl_FragCoord.z + 0.027;
}