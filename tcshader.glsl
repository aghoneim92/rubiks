#version 430

layout(vertices = 1) out;

#define ID gl_InvocationID

out gl_PerVertex{
	vec4 gl_Position;
} gl_out[];

void main()
{
    gl_out[ID].gl_Position = gl_in[ID].gl_Position;
    gl_TessLevelInner[0] = 3;
    gl_TessLevelOuter[0] = 3;
    gl_TessLevelOuter[1] = 3;
    gl_TessLevelOuter[2] = 3;
    
}
