#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec3 inNormal;

layout (binding = 0) uniform BuildIn 
{
    mat4 matrixObjectToNDC;
    mat4 matrixObjectToWorld;
    vec4 viewPos;
} _buildIn;

#define MAX_LIGHT_COUNT 2
struct PointLight
{
    mat4 lightTransform;
    float radius;
};

layout (binding = 1) uniform LightData {
    float lightCount;
    PointLight lights[MAX_LIGHT_COUNT];
} lightData;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outViewVec;
layout (location = 3) out vec3 outLightVec[MAX_LIGHT_COUNT];

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
    outColor = inColor;

    gl_Position = _buildIn.matrixObjectToNDC * vec4(inPos.xyz, 1.0);
    
    vec4 pos = _buildIn.matrixObjectToWorld * vec4(inPos.xyz, 1.0);
    outNormal = mat3(_buildIn.matrixObjectToWorld) * inNormal.xyz;
    // for(uint i = 0;i < lightData.lightCount && i < MAX_LIGHT_COUNT; ++i)
    // {
    //     vec4 lightPos = lightData.lights[i].lightTransform * vec4(0.0, 0.0, 0.0, 1.0);
    //     outLightVec[i] = pos.xyz - lightPos.xyz;
    // }
    outViewVec = _buildIn.viewPos.xyz - pos.xyz;
}

