#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec3 in_positions;
layout (location = 1) in vec4 in_colors;

layout(location = 0) out vec4 pass_colors;

void main()
{
    gl_Position = vec4(in_positions, 1.0);
    pass_colors = in_colors;
}
