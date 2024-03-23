#include <vgl.h>
#include <InitShader.h>
#include <vec.h>
//#include "MyColorCube.h"
#include "MyCylinder.h"

//MyColorCube cube;
MyCylinder cylinder;


bool bPlay = false;
float mytime = 0;

void myInit()
{
	//cube.init();
	cylinder.init(10);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//cube.draw(mytime);
	cylinder.draw(mytime);

	glutSwapBuffers();
}
void myIdle()
{
	if (bPlay != true) return;

	Sleep(16);
	mytime += 0.016;

	glutPostRedisplay();
}
void myKeyboard(unsigned char c, int x, int y)
{
	printf("c=%c x=%d y=%d\n", c, x, y);
	switch (c)
	{
	case ' ':	bPlay = !bPlay;						break;
	case '1':	cylinder.decreasePoly();			break;
	case '2':	cylinder.incresePoly();				break;
	default:										break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Color Cube Example");

	glewExperimental = true;
	glewInit();

	myInit();

	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);
	glutKeyboardFunc(myKeyboard);


	glutMainLoop();

	return 0;
}