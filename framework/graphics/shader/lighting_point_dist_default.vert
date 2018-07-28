#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
        
layout (location = 0) in vec3 inPos;

layout (binding = 0) uniform BuildIn
{
    mat4 matrixToWorld;
    mat4 matrixToView;
    mat4 matrixProjection;
    vec3 lightPos; //viewer pos
    float _dumy_w;
    mat4 cubeFaceTransform[6];
    bool rightHand;
    float depthNear;
    float depthFar;
    float _dumy_w_1;
} _buildIn;

layout (location = 0) out vec3 outWorldPos;
layout (location = 1) out vec3 outViewPos;

void main()
{
    vec4 worldPos = _buildIn.matrixToWorld * vec4(inPos.xyz, 1.0);
    outWorldPos = worldPos.xyz;
    vec4 viewPos = _buildIn.matrixToView * vec4(inPos.xyz, 1.0);
    outViewPos = viewPos.xyz;
}