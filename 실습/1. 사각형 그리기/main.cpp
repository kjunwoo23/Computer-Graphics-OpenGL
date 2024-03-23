//실습1: 사각형 그리기
#include <vgl.h>
#include <math.h>
#include <stdio.h>

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	//버퍼에 있는 그림 다 지워
	glBegin(GL_TRIANGLES);	//삼각형 그리기
	glColor3f(1, 1, 1);

	glVertex2f(-0.5, -0.5);	//좌표
	glVertex2f(0.5, -0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(-0.5, 0.5);

	glEnd();
	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);	//화면 크기
	glutCreateWindow("Hello GL");	//창 만들기, callback(미리 상황을 만들어두고, 이 상황이 닥치면 뭘할까를 사용자한테 입력받음)

	//modern opengl 사용하려면 필요
	//glewExperimental = true;
	//glewInit();

	//opengl 버전 보기
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	glutDisplayFunc(display);	//display callback
	glutMainLoop();	//여기서 멈추고 while문을 돌며 사용자 상호작용을 기다림



	return 0;
}