//실습5: 랜덤 점 찍기 - 첫 1회만 점 위치 계산
#include <vgl.h>
#include <math.h>
#include <stdio.h>


struct vec2 {
	float x;
	float y;
};

const int num = 20000;
vec2 points[num];

void init() { //데이터 포장하기
	for (int i = 0; i < num; i++) {
		points[i].x = (rand() % 3000) / 1500.0f - 1.0f;
		points[i].y = (rand() % 3000) / 1500.0f - 1.0f;
	}
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	//버퍼에 있는 그림 다 지워
	glColor3f(1, 1, 1);

	glBegin(GL_POINTS);

	for (int i = 0; i < num; i++)
		glVertex2f(points[i].x, points[i].y);
	
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


	init();
	glutDisplayFunc(display);	//display callback
	glutMainLoop();	//여기서 멈추고 while문을 돌며 사용자 상호작용을 기다림



	return 0;
}