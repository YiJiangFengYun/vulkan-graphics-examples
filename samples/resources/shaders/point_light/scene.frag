#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#define EPSILON 0.001
#define MAX_LIGHT_COUNT 2

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inViewVec;
layout (location = 3) in vec3 inLightVec[MAX_LIGHT_COUNT];

struct PointLight
{
    mat4 lightTransform;
    float radius;
    float _dummy_y;
    float _dummy_z;
    float _dummy_w;
};

layout (binding = 1) uniform LightData {
    uint lightCount;
    float _dummy_y;
    float _dummy_z;
    float _dummy_w;
    PointLight lights[MAX_LIGHT_COUNT];
} lightData;

layout (location = 0) out vec4 outFragColor;

layout (binding = 2) uniform samplerCube shadowMaps[MAX_LIGHT_COUNT];

void main() 
{   
    outFragColor = vec4(0.0, 0.0, 0.0, 1.0);
    for(uint i = 0; i < lightData.lightCount && i < MAX_LIGHT_COUNT; ++i)
    {
        vec3 resultColor = vec3(0.0);
        vec3 N = normalize(inNormal);
        vec3 L = normalize(inLightVec[i]);
        vec3 V = normalize(inViewVec);
        vec3 R = normalize(reflect(L, N));
        vec3 diffuse = max(dot(N, -L), 0.0) * inColor;
        resultColor.rgb = diffuse.rgb;
        // Shadow
        float sampledDist = texture(shadowMaps[i], L).r;
        float dist = length(inLightVec[i]);
        // Check if fragment is in shadow
        float shadow = (dist <= sampledDist + EPSILON) ? 1.0 : 0.1;
        // Check if out of light area.
        float radius = lightData.lights[i].radius;
        float strength = (dist <= radius + EPSILON) ? 1.0 : 0.1;
        resultColor.rgb *= shadow * strength;
        outFragColor.rgb += resultColor.rgb;

        outFragColor.rgb = vec3(sampledDist);
    }
    
}