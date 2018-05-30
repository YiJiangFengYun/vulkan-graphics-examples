#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

layout(binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
    mat4 matrixObjectToWorld;    
} _buildIn;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outWorldPos;
// layout (location = 3) out vec3 outTangent;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() 
{
    gl_Position = _buildIn.matrixObjectToNDC * inPos;
    
    // Vertex position in world space
    outWorldPos = vec3(_buildIn.matrixObjectToWorld * inPos);
    // // GL to Vulkan coord space
    // outWorldPos.y = -outWorldPos.y;
    
    // Normal in world space
    // mat3 mNormal = transpose(inverse(mat3(_buildIn.matrixObjectToWorld)));
    outNormal = mat3(_buildIn.matrixObjectToWorld) * normalize(inNormal);    
    
    // Currently just vertex color
    outColor = inColor;
}
