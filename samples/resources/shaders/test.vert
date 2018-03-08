#version 450

layout (location = 0) in vec2 inPos;

layout(binding = 0) uniform BuildIn {
    mat4 matrixObjectToNDC;
	vec4 mainColor;
	mat4 matrixObjectToView;
	mat4 matrixObjectToWorld;
} _buildIn;

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
    gl_Position = vec4(positions[gl_VertexIndex % 3], 1.0);
}