#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec3 inNormal;

layout(set = 0, binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
} _buildIn;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec4 outPos;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() 
{
    outUV = inUV;
    outPos = _buildIn.matrixObjectToNDC * vec4(inPos.xyz, 1.0);
    gl_Position = outPos;        
}
