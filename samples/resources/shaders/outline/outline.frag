#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 2) uniform OutlineInfoFrag
{
	vec4 outlineColor;
} outlineInfoFrag;

layout (location = 0) out vec4 outFragColor;

void main() 
{
	outFragColor = outlineInfoFrag.outlineColor;
}