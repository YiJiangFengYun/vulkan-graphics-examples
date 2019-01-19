#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec3 inNormal;

layout (set = 0, binding = 0) uniform BuildIn 
{
    mat4 matrixObjectToNDC;
    mat4 matrixObjectToWorld;
    vec4 viewPos;
} _buildIn;

#define MAX_LIGHT_COUNT 2
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

struct AmbientLight
{
    mat4 lightTransform;
    vec3 strength;
    float _dummy_w;
};

layout (set = 0, binding = 1) uniform LightData {
    uint lightCount;
    float _dummy_y;
    float _dummy_z;
    float _dummy_w;
    SpotLight lights[MAX_LIGHT_COUNT];
    uint ambientLightCount;
    float _dummy_y_2;
    float _dummy_z_2;
    float _dummy_w_2;
    AmbientLight ambientLight;
} lightData;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outViewVec;
layout (location = 3) out vec3 outLightVec[MAX_LIGHT_COUNT];
layout (location = 5) out vec4 outShadowCoord[MAX_LIGHT_COUNT];

out gl_PerVertex 
{
    vec4 gl_Position;
};

const mat4 texMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 );

void main() 
{
    outColor = inColor;

    gl_Position = _buildIn.matrixObjectToNDC * vec4(inPos.xyz, 1.0);
    
    vec4 pos = _buildIn.matrixObjectToWorld * vec4(inPos.xyz, 1.0);
    outNormal = mat3(_buildIn.matrixObjectToWorld) * inNormal.xyz;
    outViewVec = _buildIn.viewPos.xyz - pos.xyz;
     for(uint i = 0; i < lightData.lightCount && i < MAX_LIGHT_COUNT; ++i)
    {
        vec4 lightPos = lightData.lights[i].lightTransform * vec4(0.0, 0.0, 0.0, 1.0);
        outLightVec[i] = pos.xyz - lightPos.xyz;
        mat4 lightViewMatrix = inverse(lightData.lights[i].lightTransform);
        mat4 lightTVP = texMat * lightData.lights[i].lightProjection * lightViewMatrix;
        outShadowCoord[i] = lightTVP * pos;
    }
}

