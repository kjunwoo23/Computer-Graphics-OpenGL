#include <vgl.h>
#include <math.h>
#include <stdio.h>
#include <InitShader.h>


struct vec2 {
	float x;
	float y;
};

const int num = 20000;
vec2 points[num];

void init() {
	vec2 points[num];

	for (int i = 0; i < num; i++)
	{
		points[i].x = (rand() % 200) / 100.0f - 1.0f;
		points[i].y = (rand() % 200) / 100.0f - 1.0f;
	}

	//1. Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//2. Create and initialize a buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//3. Send the data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	//4. load vertex and fragment shader
	GLuint prog	= InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(prog);

	// 5. connect my data to vertex shader
	int vPosition = 0;
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_POINTS, 0, num);
	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);	//화면 크기
	glutCreateWindow("Hello GL");	//창 만들기, callback(미리 상황을 만들어두고, 이 상황이 닥치면 뭘할까를 사용자한테 입력받음)


	//modern opengl 사용하려면 필요
	glewExperimental = true;
	glewInit();

	//opengl 버전 보기
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));


	init();
	glutDisplayFunc(display);	//display callback
	glutMainLoop();	//여기서 멈추고 while문을 돌며 사용자 상호작용을 기다림



	return 0;
}