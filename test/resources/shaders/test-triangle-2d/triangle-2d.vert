#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
	vec4 mainColor;
	mat4 matrixObjectToView;
	mat4 matrixObjectToWorld;
} _buildIn;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

vec3 positions[3] = vec3[](
    vec3(0.0, -0.5, 0.0),
    vec3(0.5, 0.5, 0.0),
    vec3(-0.5, 0.5, 0.0)
);

vec4 colors[3] = vec4[](
    vec4(1.0, 0.0, 0.0, 1.0),
    vec4(0.0, 1.0, 0.0, 1.0),
    vec4(0.0, 0.0, 1.0, 1.0)
);

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = _buildIn.matrixObjectToNDC * vec4(inPosition, 0.0, 1.0);
    // gl_Position = vec4(positions[gl_VertexIndex], 1.0);
    fragColor = _buildIn.mainColor * inColor;
    // fragColor = colors[gl_VertexIndex];
}