#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

layout(binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
	vec4 mainColor;
	mat4 matrixObjectToWorld;	
} _buildIn;

layout (location = 0) out vec4 outColor;

out gl_PerVertex 
{
    vec4 gl_Position;   
};


void main() 
{
	outColor = _buildIn.mainColor * vec4(inColor, 1.0);
	gl_Position = _buildIn.matrixObjectToNDC * vec4(inPos.xyz, 1.0);
}
