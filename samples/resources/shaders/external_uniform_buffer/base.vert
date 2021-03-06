#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

layout(set = 0, binding = 0) uniform BuildIn {
    mat4 matrixProjection;
    mat4 matrixView;
} _buildIn;

layout (set = 1, binding = 0) uniform OtherInfo 
{
    mat4 model;
} otherInfo;

layout (location = 0) out vec3 outColor;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
    outColor = inColor;
    mat4 modelView = _buildIn.matrixView * otherInfo.model;
    vec3 worldPos = vec3(modelView * vec4(inPos, 1.0));
    gl_Position = _buildIn.matrixProjection * modelView * vec4(inPos.xyz, 1.0);
}
