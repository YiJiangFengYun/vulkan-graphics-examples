#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

layout(set = 0, binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
    vec4 mainColor;
    mat4 matrixObjectToWorld;
} _buildIn;

layout(set = 1, binding = 0) uniform OtherInfo {
    vec4 viewPos;
    float lodBias;
} otherInfo;

layout (location = 0) out vec2 outUV;
layout (location = 1) out float outLodBias;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
    outUV = inUV;
    outLodBias = otherInfo.lodBias;

    vec3 worldPos = vec3(_buildIn.matrixObjectToWorld * vec4(inPos, 1.0));

    gl_Position = _buildIn.matrixObjectToNDC * vec4(inPos.xyz, 1.0);

    outNormal = mat3(_buildIn.matrixObjectToWorld) * inNormal;
    vec3 lightPos = vec3(0.0);
    vec3 lPos = lightPos;
    outLightVec = lPos - worldPos.xyz;
    outViewVec = otherInfo.viewPos.xyz - worldPos.xyz;        
}
