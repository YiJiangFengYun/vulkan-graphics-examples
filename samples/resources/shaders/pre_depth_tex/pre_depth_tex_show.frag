#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//Result texture come from pre depth render, it is buildin texture binded.
layout(set = 0, binding = 0) uniform sampler2D depthSampler;

layout(set = 1, binding = 0) uniform OtherInfo {
  float zFar;
  float zNear;    
} otherInfo;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

float LinearizeDepth(float depth)
{
  float n = otherInfo.zNear; // camera z near
  float f = otherInfo.zFar; // camera z far
  float z = depth;
  return n / (f - z * (f - n));    
}

void main() 
{
    float depth = texture(depthSampler, inUV).r;
    outColor = vec4(vec3(LinearizeDepth(depth)), 1.0);
}