#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
        
layout (location = 0) in vec4 inPos;
layout (binding = 0) uniform BuildIn
{
    mat4 matrixToNDC;
} _buildIn;

out gl_PerVertex
{
	vec4 gl_Position;
};
void main()
{
	gl_Position = _buildIn.matrixToNDC * inPos;
}