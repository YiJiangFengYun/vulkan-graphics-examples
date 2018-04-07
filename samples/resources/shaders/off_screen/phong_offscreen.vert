#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec3 inNormal;

layout(binding = 0) uniform BuildIn {
    mat4 matrixModel;
	mat4 matrixView;
	mat4 matrixProj;
} _buildIn;

layout (binding = 2) uniform OtherInfo 
{
	mat4 matrixInverse;
	vec4 lightPos;
} otherInfo;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outPos;
layout (location = 3) out vec3 outLightVec;

out gl_PerVertex
{
	vec4 gl_Position;
	float gl_ClipDistance[];
};

void main() 
{
	mat4 matrixObjectToView = _buildIn.matrixView * otherInfo.matrixInverse * _buildIn.matrixModel;
	mat4 matrixObjectToNDC = _buildIn.matrixProj * matrixObjectToView;
	outNormal = mat3(matrixObjectToView) * inNormal;
	outColor = inColor;
	gl_Position = matrixObjectToNDC * inPos;
	outPos = vec3(matrixObjectToView * inPos);
	outLightVec = normalize(otherInfo.lightPos.xyz - outPos);

	// Clip against reflection plane
	// vec4 clipPlane = vec4(0.0, -1.0, 0.0, 1.5);
	vec4 clipPlane = vec4(0.0, 1.0, 0.0, 0.0);
	gl_ClipDistance[0] = dot(inPos, clipPlane);	
}
