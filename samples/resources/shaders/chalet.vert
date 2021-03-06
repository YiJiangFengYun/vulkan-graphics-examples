#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
    vec4 mainColor;
    mat4 matrixObjectToWorld;
} _buildIn;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = _buildIn.matrixObjectToNDC * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}