#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColor;

layout(binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
    vec4 mainColor;
} _buildIn;

layout (location = 0) out vec4 outColor;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
    outColor = _buildIn.mainColor * inColor;
    gl_Position = _buildIn.matrixObjectToNDC * vec4(inPos, 1.0);
}