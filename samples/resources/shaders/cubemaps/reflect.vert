#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

layout(set = 0, binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
    mat4 matrixObjectToView;
} _buildIn;

layout(set = 1, binding = 0) uniform OtherInfo {
    float lodBias;
} otherInfo;


layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out float outLodBias;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;
layout (location = 5) out mat4 outInvModelView;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
    gl_Position = _buildIn.matrixObjectToNDC * vec4(inPos.xyz, 1.0);
    
    outPos = vec3(_buildIn.matrixObjectToView * vec4(inPos, 1.0));
    outNormal = mat3(_buildIn.matrixObjectToView) * inNormal;    
    outLodBias = otherInfo.lodBias;
    
    outInvModelView = inverse(_buildIn.matrixObjectToView);

    vec3 lightPos = vec3(0.0f, -5.0f, 5.0f);
    outLightVec = lightPos.xyz - outPos.xyz;
    outViewVec = -outPos.xyz;        
}
