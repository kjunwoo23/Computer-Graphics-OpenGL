#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include "MyCube.h"
#include "MyPyramid.h"


MyCube cube;
MyPyramid pyramid;

GLuint program;
GLuint uMat;

mat4 g_Mat = mat4(1.0);

void myInit()
{
	cube.Init();
	pyramid.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
}

float g_time = 0;

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	g_Mat = RotateY(g_time * 10.0f);


	GLuint uMat = glGetUniformLocation(program, "uMat");
		
	mat4 M(1.0);
	M = Scale(0.5, 1, 0.4);
	mat4 CTM(1.0f);					// current transform matrix (CTM)
	CTM = g_Mat;

	// BASE
	glUniformMatrix4fv(uMat, 1, true, CTM *M);
	pyramid.Draw(program);


	CTM *= Translate(0, 0.25, 0.25) * RotateZ(g_time*10.0f);

	// BLADE1
	M = Scale(0.8, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// BLADE2
	M = RotateZ(90) * M;
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// SHAFT
	M = Scale(0.1, 0.1, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);


	for (int i = 0; i < 4; i++)
	{
		M = RotateZ(i*90)*Translate(0.1, 0.25, 0)*Scale(0.2, 0.3, 0.02);
		glUniformMatrix4fv(uMat, 1, true, CTM * M);
		cube.Draw(program);
	}


	glutSwapBuffers();
}

void myIdle()
{
	g_time += 0.16f;
	Sleep(16);
	glutPostRedisplay();
}



int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Cube and Pyramid");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}