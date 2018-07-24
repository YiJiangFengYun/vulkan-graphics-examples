#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
        
layout (location = 0) in vec3 inPos;

layout (binding = 0) uniform BuildIn
{
    mat4 matrixToNDC;
    mat4 matrixToWorld;
    vec3 lightPos;
    float _dumy_w;
} _buildIn;

layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outLightPos;

out gl_PerVertex
{
    vec4 gl_Position;
};
void main()
{
    gl_Position = _buildIn.matrixToNDC * vec4(inPos, 1.0);

    vec4 tempPos = _buildIn.matrixToWorld * vec4(inPos, 1.0);
    outPos = tempPos.xyz;
    outLightPos = _buildIn.lightPos.xyz;
}