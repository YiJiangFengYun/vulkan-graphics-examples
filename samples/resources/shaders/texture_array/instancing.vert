#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;

struct Instance
{
    mat4 model;
    vec4 arrayIndex;
};

layout(set = 0, binding = 0) uniform BuildIn {
    mat4 matrixProjection;
    mat4 matrixView;
} _buildIn;

layout(set = 1, binding = 0) uniform OtherInfo {
    Instance instance[8];
} otherInfo;

layout (location = 0) out vec3 outUV;

void main() 
{
    outUV = vec3(inUV, otherInfo.instance[gl_InstanceIndex].arrayIndex.x);
    mat4 modelView = _buildIn.matrixView * otherInfo.instance[gl_InstanceIndex].model;
    gl_Position = _buildIn.matrixProjection * modelView * inPos;
}
