#include <vgl.h>
#include <InitShader.h>
#include <vec.h>
#include "MyPlain.h"

MyPlain plain;

bool bPlay = false;
float mytime = 0;

void myInit()
{
	plain.init();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	plain.draw(mytime);

	glutSwapBuffers();
}
void myIdle()
{
	if (bPlay != true) return;

	Sleep(16);
	mytime += 0.016f;
	glutPostRedisplay();
}
void myKeyboard(unsigned char c, int x, int y)
{
	if (c == ' ')
		bPlay = !bPlay;
	if (c == '1') {
		if (plain.rowNum > 2)
		{
			plain.rowNum--;
			plain.NumVertex = 6 * (int)pow(plain.rowNum, 2);
			plain.init();
			glutPostRedisplay();
		}
		printf("Division: %d, Num.of Triangles: %d, Num. of Vertices: %d\n", plain.rowNum, (int)pow(plain.rowNum, 2) * 2, plain.NumVertex);
	}
	if (c == '2') {
		plain.rowNum++;
		plain.NumVertex = 6 * (int)pow(plain.rowNum, 2);
		plain.init();
		glutPostRedisplay();
		printf("Division: %d, Num.of Triangles: %d, Num. of Vertices: %d\n", plain.rowNum, (int)pow(plain.rowNum, 2) * 2, plain.NumVertex);
	}
	if (c == 'w') {
		plain.wave = !plain.wave;
		glutPostRedisplay();
	}
	if (c == 'q') {
		exit(0);
	}
}

int main(int argc, char** argv)
{
	printf("A Waving Color Plain\n");
	printf("Programming Assignment #1 for Computer Graphics\n");
	printf("Department of Software, Sejong University\n");
	printf("\n----------------------------------------------------------------\n");
	printf("`1' key: Decreaing the Number of Division\n");
	printf("`2' key: Increaing the Number of Division\n");
	printf("`w' key: Showing/hiding the waving pattern\n");
	printf("Spacebar: starting/stoping rotating and waving\n");
	printf("\n`Q' Key: Exit the program.\n");
	printf("----------------------------------------------------------------\n\n");


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("A Waving Plain");

	glewExperimental = true;
	glewInit();

	myInit();

	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);
	glutKeyboardFunc(myKeyboard);


	glutMainLoop();

	return 0;
}