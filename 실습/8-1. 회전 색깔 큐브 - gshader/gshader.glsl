#version 330

layout (triangles) in;

in vec4 position[];
in vec4 color[];

out vec4 gColor;
out vec4 gPosition;
layout (line_strip, max_vertices = 6) out;

void main()
{

	for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
	    gColor = color[i];
	    gPosition = position[i];
        EmitVertex();
    }

	for (int i = gl_in.length() - 1; i >= 0; i--)
    {
        gl_Position = gl_in[i].gl_Position * 0.7f;
        gl_Position.w = 1.0f;
	    gColor = color[i];
        EmitVertex();
    }
    
    EndPrimitive();
}


/*
in gl_Vertex
{
    vec4  gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];
*/