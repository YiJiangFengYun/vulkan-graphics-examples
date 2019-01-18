#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 0, binding = 0) uniform sampler2D colorSampler;
layout (set = 0, binding = 1) uniform sampler2D depthSampler;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

// float LinearizeDepth(float depth)
// {
//   float n = 0.001; // camera z near
//   float f = 256.0; // camera z far
//   float z = depth;
//   return n / (f - z * (f - n));    
// }

void main() 
{
    vec4 color = texture(colorSampler, inUV);
    float depth = texture(depthSampler, inUV).r;
    gl_FragDepth = depth;
    outColor = color;
}