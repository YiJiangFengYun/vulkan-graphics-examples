#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform BuildIn 
{
	mat4 matrixToNDC;
	mat4 matrixToWorld;
	vec4 lightPos;	
} _buildIn;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outLightVec;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	outColor = inColor;
	gl_Position = _buildIn.matrixToNDC * vec4(inPos.xyz, 1.0);
	outNormal = mat3(_buildIn.matrixToWorld) * inNormal;
	vec4 pos = _buildIn.matrixToWorld * vec4(inPos, 1.0);
	vec4 lPos = _buildIn.matrixToWorld * _buildIn.lightPos;
	outLightVec = vec3(lPos - pos);
}