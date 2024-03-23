//실습2: 무지개 다각형 그리기
#include <vgl.h>
#include <math.h>
#include <stdio.h>

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	//버퍼에 있는 그림 다 지워
	
	int div = 10;
	float radius = 0.7f;
	float dtheta = 2 * 3.141592 / div;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < div; i++) {
		glColor3f((float)i / div, 0, 1 - (float)i / div);	//색깔
		float x1 = radius * cos(i * dtheta);
		float y1 = radius * sin(i * dtheta);
		float x2 = radius * cos((i + 1) * dtheta);
		float y2 = radius * sin((i + 1) * dtheta);
		glVertex2f(0, 0);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	}

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