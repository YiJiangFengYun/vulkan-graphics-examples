#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec3 inNormal;

layout(binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
	mat4 matrixObjectToView;
} _buildIn;

layout (binding = 1) uniform OtherInfo 
{
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
	outNormal = mat3(_buildIn.matrixObjectToView) * inNormal;
	outColor = inColor;
	gl_Position = _buildIn.matrixObjectToNDC * inPos;
	outPos = vec3(_buildIn.matrixObjectToView * inPos);
	outLightVec = normalize(otherInfo.lightPos.xyz - outPos);

	// Clip against reflection plane
	// vec4 clipPlane = vec4(0.0, -1.0, 0.0, 1.5);
	vec4 clipPlane = vec4(0.0, 1.0, 0.0, 0.0);
	gl_ClipDistance[0] = dot(inPos, clipPlane);	
}
