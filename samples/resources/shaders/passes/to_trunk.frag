#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform sampler2D color;
layout (binding = 1) uniform sampler2D depth;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

void main() 
{
	vec4 color = texture(color, inUV);
	float depth = texture2D(depth, inUV).r;

	gl_FragCoord.z = depth;
	outColor = color;
}