#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inPos;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform BuildIn 
{
	mat4 matrixToNDC;
} _buildIn;

layout (binding = 1) uniform OutlineInfoVert
{
	float outlineWidth;
} outlineInfoVert;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	// Extrude along normal
	vec4 pos = vec4(inPos.xyz + inNormal * outlineInfoVert.outlineWidth, inPos.w);
	gl_Position = _buildIn.matrixToNDC * pos;
}
