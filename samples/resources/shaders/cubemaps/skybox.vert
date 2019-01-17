#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;

layout(set = 0, binding = 0) uniform BuildIn {
    mat4 matrixWorld;
    mat4 matrixView;
    mat4 matrixProjection;
} _buildIn;

layout (location = 0) out vec3 outUVW;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
    outUVW = inPos;
    //outUVW.x *= -1.0;
    // gl_Position = vec4(inPos.xyz, 1.0);
    gl_Position = _buildIn.matrixProjection * mat4(mat3(_buildIn.matrixView)) * mat4(mat3(_buildIn.matrixWorld)) * vec4(inPos.xyz, 1.0);
}
