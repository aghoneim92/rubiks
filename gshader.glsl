#version 430

uniform mat4 model_view;
uniform mat4 projection;

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main()
{
	int i;	
	
	for(i=0; i<3; i++){
		gl_Position.x = gl_in[i].gl_Position.x*cos(gl_in[i].gl_Position.y*(3.14195/4)/(150));
		gl_Position.yzw = gl_in[i].gl_Position.yzw;
		gl_Position = projection * model_view* gl_Position;
		EmitVertex();
	}
}
