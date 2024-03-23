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

mat4 myLookAt(vec3 e, vec3 f, vec3 u)
{
	vec3 n = normalize(f - e);			// -Cz direction
	u = normalize(u);
	vec3 a = dot(u, n) * n;
	vec3 v = normalize(u - a);			// Cy direction
	vec3 w = normalize(cross(v, -n));	// Cx direction

	mat4 Rw(1.0f);
	Rw[0][0] = w.x;		Rw[0][1] = v.x;		Rw[0][2] = -n.x;
	Rw[1][0] = w.y;		Rw[1][1] = v.y;		Rw[1][2] = -n.y;
	Rw[2][0] = w.z;		Rw[2][1] = v.z;		Rw[2][2] = -n.z;

	mat4 Rc(1.0f);
	Rc[0][0] = w.x;		Rc[0][1] = w.y;		Rc[0][2] = w.z;
	Rc[1][0] = v.x;		Rc[1][1] = v.y;		Rc[1][2] = v.z;
	Rc[2][0] = -n.x;		Rc[2][1] = -n.y;		Rc[2][2] = -n.z;

	mat4 Tc = Translate(-e.x, -e.y, -e.z);
	//	mat4 Rc = transpose(Rw);

	mat4 out = Rc * Tc;
	return out;

}

float g_time = 0;

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	GLuint uMat = glGetUniformLocation(program, "uMat");

	vec3 eye = vec3(3 * cos(g_time * 3.141592 / 2), 3 + 3 * sin(g_time * 3.141592 / 4), 3 * sin(g_time * 3.141592 / 2));
	vec3 at = vec3(0, 0, 0);
	vec3 up = vec3(0, 1, 0);

	mat4 ViewM = myLookAt(eye, at, up);// view matrix


	mat4 M(1.0);
	mat4 CTM(1.0);	// current transform matrix

	glUniformMatrix4fv(uMat, 1, true, ViewM * CTM * M);
	cube.Draw(program);

	glutSwapBuffers();
}

void myIdle()
{
	g_time += 0.016f;
	Sleep(16);
	glutPostRedisplay();
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Cube and Pyramid");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}