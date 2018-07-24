#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inLightPos;

layout (location = 0) out float outFragColor;

void main() 
{
    vec3 lightVec = inPos.xyz - inLightPos.xyz;
    outFragColor = length(lightVec);
}