#include <vgl.h>
#include <InitShader.h>
#include <vec.h>

vec4 vertex_pos[8] = {
	vec4(-0.5,-0.5, 0.5, 1),
	vec4(-0.5, 0.5, 0.5, 1),
	vec4(0.5, 0.5, 0.5, 1),
	vec4(0.5,-0.5, 0.5, 1),
	vec4(-0.5,-0.5,-0.5, 1),
	vec4(-0.5, 0.5,-0.5, 1),
	vec4(0.5, 0.5, -0.5, 1),
	vec4(0.5,-0.5, -0.5, 1)
};

vec4 vertex_color[8] = {
	vec4(1,0,0,1),
	vec4(0,1,0,1),
	vec4(0,0,1,1),
	vec4(1,1,0,1),
	vec4(0,1,1,1),
	vec4(1,0,1,1),
	vec4(1,1,1,1),
	vec4(0,0,0,1)
};

const int NumVertex = 6 * 2 * 3;
vec4 positions[NumVertex];
vec4 colors[NumVertex];


void makeRect(int a, int b, int c, int d)
{
	static int cur = 0;
	positions[cur] = vertex_pos[a];	colors[cur] = vertex_color[a]; cur++;
	positions[cur] = vertex_pos[b];	colors[cur] = vertex_color[b]; cur++;
	positions[cur] = vertex_pos[c];	colors[cur] = vertex_color[c]; cur++;

	positions[cur] = vertex_pos[c];	colors[cur] = vertex_color[c]; cur++;
	positions[cur] = vertex_pos[d];	colors[cur] = vertex_color[d]; cur++;
	positions[cur] = vertex_pos[a];	colors[cur] = vertex_color[a]; cur++;
}



void makeCube()
{
	makeRect(0, 3, 2, 1);
	makeRect(4, 5, 6, 7);
	makeRect(0, 4, 7, 3);
	makeRect(3, 7, 6, 2);
	makeRect(2, 6, 5, 1);
	makeRect(1, 5, 4, 0);

}

void myInit()
{
	makeCube();

	//1. Setup VAO (gen and bind)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//2. Setup VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//3. Copy data to Buffer

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * NumVertex * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(vec4) * NumVertex, positions);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * NumVertex,
		sizeof(vec4) * NumVertex, colors);

	//4. Load Shaders

	GLuint prog = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(prog);

	//5. Connect shader to buffer
	// vPosition
	GLuint vPosition = glGetAttribLocation(prog, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// vColor
	GLuint vColor = glGetAttribLocation(prog, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(positions)));

}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, NumVertex);
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Color Cube Example");

	glewExperimental = true;
	glewInit();

	myInit();

	glutDisplayFunc(myDisplay);
	glutMainLoop();

	return 0;
}