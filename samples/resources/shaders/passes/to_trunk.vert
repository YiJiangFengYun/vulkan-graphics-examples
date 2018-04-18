#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUV; 

layout (location = 0) out vec2 outUV;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	outUV = inUV;
	gl_Position = vec4(inPos, 1.0, 1.0);
}