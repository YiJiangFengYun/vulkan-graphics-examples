#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#define TAN_HALF_FOV 1.f // tan(45deg)
#define ASPECT 1.f

layout(triangles) in;

layout (binding = 0) uniform BuildIn
{
    mat4 matrixToWorld;
    mat4 matrixToView;
    mat4 matrixProjection;
    vec3 lightPos; //viewer pos
    float _dumy_w;
    mat4 cubeFaceTransform[6];
    bool rightHand;
    float depthNear;
    float depthFar;
    float _dumy_w_1;
} _buildIn;

layout(location = 0) in vec3 inWorldPoses[];
layout(location = 1) in vec3 inViewPoses[];

layout(triangle_strip, max_vertices = 18) out;
out gl_PerVertex
{
    vec4 gl_Position;
};
layout(location = 0) out vec3 outWorldPos;

bool inView(vec4 view_pos)
{
    uint res = 1;
    if (_buildIn.rightHand) 
    {
        res *= uint(step(_buildIn.depthNear, view_pos.y));
        res *= uint(step(view_pos.y, _buildIn.depthFar));
        float zMax = TAN_HALF_FOV * view_pos.y;
        float xMax = zMax * ASPECT;
        res *= uint(step(-xMax, view_pos.x));
        res *= uint(step(view_pos.x, xMax));
        res *= uint(step(-zMax, view_pos.z));
        res *= uint(step(view_pos.z, zMax));
    }
    else
    {
        res *= uint(step(_buildIn.depthNear, view_pos.z));
        res *= uint(step(view_pos.z, _buildIn.depthFar));
        float yMax = TAN_HALF_FOV * view_pos.z;
        float xMax = yMax * ASPECT;
        res *= uint(step(-xMax, view_pos.x));
        res *= uint(step(view_pos.x, xMax));
        res *= uint(step(-yMax, view_pos.y));
        res *= uint(step(view_pos.y, yMax));
    }
    return bool(res);
}

void emitLayerVertices(int layer)
{
    gl_Layer = layer;
    vec4 pos0 = _buildIn.cubeFaceTransform[layer] * vec4(inViewPoses[0].xyz, 1.0);
    vec4 pos1 = _buildIn.cubeFaceTransform[layer] * vec4(inViewPoses[1].xyz, 1.0);
    vec4 pos2 = _buildIn.cubeFaceTransform[layer] * vec4(inViewPoses[2].xyz, 1.0);
    // if (inView(pos0) || inView(pos1) || inView(pos2)) 
    {
        gl_Position = _buildIn.matrixProjection * pos0;
        outWorldPos = inWorldPoses[0];
        EmitVertex();

        gl_Position = _buildIn.matrixProjection * pos1;
        outWorldPos = inWorldPoses[1];
        EmitVertex();

        gl_Position = _buildIn.matrixProjection * pos2;
        outWorldPos = inWorldPoses[2];
        EmitVertex();

        EndPrimitive();
    }
}

void main()
{
    //positive x
    emitLayerVertices(0);

    //negative x
    emitLayerVertices(1);

    //positive y
    emitLayerVertices(2);

    //negative y
    emitLayerVertices(3);

    //positive z
    emitLayerVertices(4);

    //negative z
    emitLayerVertices(5);

}