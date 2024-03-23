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
float ang1 = 0.0f;
float ang2 = 0.0f;
float ang3 = 0.0f;

void drawRobotArm1()
{
	mat4 M(1.0);
	mat4 CTM(1.0);	// current transform matrix

	GLuint uMat = glGetUniformLocation(program, "uMat");

	// BASE
	CTM = Translate(0, -0.5, 0) * RotateY(g_time * 90);
	M = Scale(0.3, 0.2, 0.2);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	// UPPER ARM
	CTM = CTM * RotateZ(ang1);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// LOWER ARM
	CTM = CTM * Translate(0, 0.4, 0) * RotateZ(ang2);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// HAND
	CTM = CTM * Translate(0, 0.4, 0) * RotateZ(ang3);
	M = Translate(0, 0.05, 0) * Scale(0.3, 0.1, 0.2);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);
}


#include <list>

void drawRobotArm2()
{
	std::list<mat4> stack;

	mat4 M(1.0);
	mat4 CTM(1.0);	// current transform matrix

	GLuint uMat = glGetUniformLocation(program, "uMat");

	stack.push_back(CTM);
	// BASE
	CTM = Translate(0, -0.5, 0) * RotateY(g_time * 90);
	M = Scale(0.3, 0.2, 0.2);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	stack.push_back(CTM);
	// UPPER ARM
	CTM = CTM * RotateZ(ang1);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	stack.push_back(CTM);
	// BRIDGE
	CTM = CTM * Translate(0, 0.4, 0) * RotateZ(ang2);
	M = Translate(0, 0.05, 0) * Scale(0.4, 0.1, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	stack.push_back(CTM);
	// LOWER ARM 1
	CTM = CTM * Translate(-0.2, 0.05, 0) * RotateZ(ang3);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	CTM = stack.back();
	stack.pop_back();

	stack.push_back(CTM);
	// LOWER ARM 2
	CTM = CTM * Translate(0.2, 0.05, 0) * RotateZ(-ang3);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	CTM = stack.back();
	stack.pop_back();

/*
	stack.push_back(CTM);
	// LOWER ARM 3
	CTM = CTM * Translate(0.0, 0.0, 0) * RotateZ(0);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);
*/

}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

		
	ang1 = 45*sin(g_time*3.141592);
	ang2 = -45*sin(g_time * 3.141592*2);
	ang3 = 30* sin(g_time * 3.141592);
	
	drawRobotArm2();

	glutSwapBuffers();
}

void myIdle()
{
	g_time += 0.016f;
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