#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#define EPSILON 0.001
#define MAX_LIGHT_COUNT 2

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inViewVec;
layout (location = 3) in vec3 inLightVec[MAX_LIGHT_COUNT];
layout (location = 5) in vec4 inShadowCoord[MAX_LIGHT_COUNT];

struct SpotLight
{
    mat4 lightTransform;
    mat4 lightProjection;
    float radius;
    float _dummy_y;
    float _dummy_z;
    float _dummy_w;
    vec3 strength;
    float _dumy_w_2;
};

layout (binding = 1) uniform LightData {
    uint lightCount;
    float _dummy_y;
    float _dummy_z;
    float _dummy_w;
    SpotLight lights[MAX_LIGHT_COUNT];
} lightData;

layout (location = 0) out vec4 outFragColor;

layout (binding = 2) uniform sampler2D shadowMaps[MAX_LIGHT_COUNT];

float textureProj(uint index, vec4 P, vec2 off)
{
	vec4 shadowCoord = P / P.w;
	if ( shadowCoord.z > 0.0 && shadowCoord.z < 1.0 ) 
	{
	    float strength = 1.0;
		float dist = texture(shadowMaps[index], shadowCoord.st + off ).r;
		if ( shadowCoord.w > 0.0 && dist < shadowCoord.z ) 
		{
			strength = 0.0;
		}
	    return strength;
	}
    return 0.0;
}

void main() 
{   
    outFragColor = vec4(0.0, 0.0, 0.0, 1.0);
    for(uint i = 0; i < lightData.lightCount && i < MAX_LIGHT_COUNT; ++i)
    {
        float strength = textureProj(i, inShadowCoord[i], vec2(0.0));

        vec3 resultColor = vec3(0.0);
        vec3 N = normalize(inNormal);
        vec3 L = normalize(inLightVec[i]);
        vec3 V = normalize(inViewVec);
        vec3 R = normalize(reflect(L, N));
        vec3 diffuse = max(dot(N, -L), 0.0) * inColor;
        resultColor = diffuse;
        float dist = length(inLightVec[i]);
        // Check if out of light area.
        float radius = lightData.lights[i].radius;
        strength = (max(radius - dist, 0.0)) / radius * strength;
        resultColor.rgb *= strength * lightData.lights[i].strength.rgb;
        outFragColor.rgb += resultColor;
    }
}