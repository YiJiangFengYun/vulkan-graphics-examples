#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//Result of post render.
layout (set = 0, binding = 0) uniform sampler2D samplerColorMap;

layout (set = 1, binding = 0) uniform MutiplyColorInfo 
{
    vec4 color;
} mutiplyColorInfo;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = texture(samplerColorMap, inUV) * mutiplyColorInfo.color;
}