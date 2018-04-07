#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
	vec4 mainColor;
	mat4 matrixObjectToView;
	mat4 matrixObjectToWorld;
} _buildIn;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = _buildIn.matrixObjectToNDC * vec4(inPosition, 1.0);
    fragColor = _buildIn.mainColor * inColor;
	//gl_VertexIndex
}