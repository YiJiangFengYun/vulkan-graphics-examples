#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform MainColor {
    vec4 color;
} mainColor;
layout(binding = 2) uniform MatrixObjectToNDC {
    mat4 matrix;
} _matrixObjectToNDC;
layout(binding = 3) uniform MatrixObjectToWorld {
    mat4 matrix;
} _matrixObjectToWorld;
layout(binding = 4) uniform MatrixObjectToView
{
    mat4 matrix;
} _matrixObjectToView;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = _matrixObjectToNDC.matrix * vec4(inPosition, 1.0);
	fragColor = vec3(1.0, 1.0, 1.0);
	fragTexCoord = inTexCoord;
}