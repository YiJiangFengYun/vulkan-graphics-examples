#version 450

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec4 inColor;

layout(binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
	vec4 mainColor;
	mat4 matrixObjectToWorld;    
} _buildIn;

// layout (push_constant) uniform PushConstants {
// 	vec2 scale;
// 	vec2 translate;
// } pushConstants;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec4 outColor;

out gl_PerVertex 
{
	vec4 gl_Position;   
};

vec3 positions[3] = vec3[](
    vec3(0.0, -0.5, 0.0),
    vec3(0.5, 0.5, 0.0),
    vec3(-0.5, 0.5, 0.0)
);

vec4 colors[3] = vec4[](
    vec4(1.0, 1.0, 0.0, 1.0),
    vec4(1.0, 1.0, 0.0, 1.0),
    vec4(0.0, 0.0, 1.0, 1.0)
);

void main() 
{
	outUV = inUV;
    outColor = _buildIn.mainColor * inColor;
    // outColor = colors[gl_VertexIndex % 3];
	gl_Position = _buildIn.matrixObjectToNDC * vec4(inPos, 0.0, 1.0);
	// gl_Position = vec4(inPos * pushConstants.scale + pushConstants.translate, 0.0, 1.0);
    // gl_Position = vec4(positions[gl_VertexIndex % 3], 1.0);
}