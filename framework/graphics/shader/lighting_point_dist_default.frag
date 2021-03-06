#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 0, binding = 0) uniform BuildIn
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

layout (location = 0) in vec3 inWorldPos;

layout (location = 0) out float outFragColor;

void main() 
{
    float d = distance(inWorldPos.xyz, _buildIn.lightPos.xyz);
    outFragColor = d;
}