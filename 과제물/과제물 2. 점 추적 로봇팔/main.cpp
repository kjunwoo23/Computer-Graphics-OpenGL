#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include "MyCube.h"
#include "MyPyramid.h"
#include "MyTarget.h"

MyCube cube;
MyPyramid pyramid;
MyTarget target(&cube);

GLuint program;
GLuint uMat;

mat4 CTM;

bool bPlay = false;
bool bChasingTarget = false;
bool bDrawTarget = false;

float ang1 = 0;
float ang2 = 0;
float ang3 = 0;

void myInit()
{
	cube.Init();
	pyramid.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
}

float g_time = 0;

void drawRobotArm(float ang1, float ang2, float ang3)
{

	mat4 temp = CTM;

	mat4 M(1.0);

	// base 1
	M = Translate(0, 0, 0.075) * Scale(0.3, 0.3, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	// base 2
	M = Translate(0, 0, -0.075) * Scale(0.3, 0.3, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	// base bolt
	M = Scale(0.05, 0.05, 0.25);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// Upper Arm
	CTM *= RotateZ(ang1);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.55, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// Lower Arm 1
	CTM *= Translate(0, 0.4, 0) * RotateZ(ang2);
	M = Translate(0, 0.2, 0.065) * Scale(0.1, 0.5, 0.03);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// Lower Arm 2
	M = Translate(0, 0.2, -0.065) * Scale(0.1, 0.5, 0.03);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// Lower bolt
	M = Scale(0.05, 0.05, 0.25);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// Hand
	CTM *= Translate(0, 0.4, 0) * RotateZ(ang3);
	M = Scale(0.4, 0.15, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// Hand bolt
	M = Scale(0.05, 0.05, 0.25);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	// Hair
	M = Translate(0, 0.1, 0) * Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	CTM *= Translate(0.225, 0, 0);
	// Eye, Tongue
	M = Scale(0.5, 0.01, 0.01);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	CTM *= Translate(0.05, 0, 0);
	// Mouth
	for (int i = 0; i < 4; i++) {
		M = RotateX(g_time * 360 + 90 * i) * RotateZ(30 * sin(g_time * 3.141592) + 30) * Translate(0, 0.1, 0) * Scale(0.25, 0.05, 0.1);
		glUniformMatrix4fv(uMat, 1, true, CTM * M);
		pyramid.Draw(program);
	}

	CTM = temp;
}

vec3 calcEyePos(float a1, float a2, float a3) {
	
	vec3 eyePos = 0;;
	eyePos.x = -0.4 * sin(a1 * 3.141592 / 180)
		- 0.4 * sin((a1 + a2) * 3.141592 / 180)
		+ 0.2 * cos((a1 + a2 + a3) * 3.141592 / 180);
	eyePos.y = 0.4 * cos(a1 * 3.141592 / 180)
		+ 0.4 * cos((a1 + a2) * 3.141592 / 180)
		+ 0.2 * sin((a1 + a2 + a3) * 3.141592 / 180);
	return eyePos;
}

mat4 calcEyeMat() {
	mat4 eyeMat(1.0);
	eyeMat = CTM * eyeMat;
	return eyeMat;
}

float calcDistance(vec3 eyePos) {
	vec3 redPos = target.GetPosition(g_time);

	vec3 tmp;
	tmp = eyePos - redPos;
	float dist = sqrt(tmp.x * tmp.x + tmp.y * tmp.y);
	return dist;
}

float prevEyeDist = 1;
float prevHandBoltDist;
float prevArmBoltDist;
float delta = 3.141592 / 180 * 25;

vec3 calcArmBoltPosition(float a1) {
	vec3 pos;
	pos.x = -0.4 * sin(a1 * 3.141592 / 180);
	pos.y = 0.4 * cos(a1 * 3.141592 / 180);
	pos.z = 0;
	return pos;
}

void moveUpperArm() {
	float nextBoltDist = 1;
	int dir;	

	prevArmBoltDist = calcDistance(calcArmBoltPosition(ang1));
	if (calcDistance(calcArmBoltPosition(ang1 - delta * 0.15 * pow(prevArmBoltDist, 2)))
		> calcDistance(calcArmBoltPosition(ang1 + delta * 0.15 * pow(prevArmBoltDist, 2))))
		dir = 1;
	else dir = -1;

	nextBoltDist = calcDistance(calcArmBoltPosition(ang1 + delta * 0.15 * pow(prevArmBoltDist, 2) * dir));
	if (nextBoltDist > 0.6 && prevArmBoltDist > nextBoltDist) {
		ang1 += delta * 0.15 * pow(prevArmBoltDist, 2) * dir;
		prevArmBoltDist = calcDistance(calcArmBoltPosition(ang1));
	}
	else if (nextBoltDist < 0.2 && prevArmBoltDist > nextBoltDist) {
		ang1 -= delta * 0.15 * pow(prevArmBoltDist, 2) * dir;
		prevArmBoltDist = calcDistance(calcArmBoltPosition(ang1));
	}
}

vec3 calcHandBoltPosition(float a1, float a2) {
	vec3 pos;
	pos.x = -0.4 * sin(a1 * 3.141592 / 180)
		- 0.4 * sin((a1 + a2) * 3.141592 / 180);
	pos.y = 0.4 * cos(a1 * 3.141592 / 180)
		+ 0.4 * cos((a1 + a2) * 3.141592 / 180);
	pos.z = 0;
	return pos;
}

void moveLowerArm() {
	float nextBoltDist = 1;
	int dir;

	prevHandBoltDist = calcDistance(calcHandBoltPosition(ang1, ang2));
	if (calcDistance(calcHandBoltPosition(ang1, ang2 - delta * pow(prevHandBoltDist, 2)))
		> calcDistance(calcHandBoltPosition(ang1, ang2 + delta * pow(prevHandBoltDist, 2))))
		dir = 1;
	else dir = -1;

	nextBoltDist = calcDistance(calcHandBoltPosition(ang1, ang2 + delta * pow(prevHandBoltDist, 2) * dir));
	if (nextBoltDist > 0.21 && prevHandBoltDist > nextBoltDist)
		ang2 += delta * pow(prevHandBoltDist, 2) * dir;
	if (nextBoltDist < 0.19 && prevHandBoltDist > nextBoltDist)
		ang2 -= delta * pow(prevHandBoltDist, 2) * dir;
}



void moveHead() {
	float nextEyeDist = 1;
	int dir;

	prevEyeDist = calcDistance(calcEyePos(ang1, ang2, ang3));
	
	if (calcDistance(calcEyePos(ang1, ang2, ang3 - delta * prevEyeDist))
		> calcDistance(calcEyePos(ang1, ang2, ang3 + delta * prevEyeDist)))
		dir = 1;
	else dir = -1;

	nextEyeDist = calcDistance(calcEyePos(ang1, ang2, ang3 + delta * prevEyeDist * dir));
	if (prevEyeDist > nextEyeDist) {
		ang3 += delta * prevEyeDist * dir;
	}
}
void computeAngle()
{
	for (int i = 0; i < 50; i++) {
		moveUpperArm();
		moveLowerArm();
		moveHead();
	}
}


void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint uColor = glGetUniformLocation(program, "uColor");
	glUniform4f(uColor, -1, -1, -1, -1);


	uMat = glGetUniformLocation(program, "uMat");
	CTM = Translate(0, -0.4, 0) * RotateY(g_time * 30);
	drawRobotArm(ang1, ang2, ang3);


	glUniform4f(uColor, 1, 0, 0, 1);
	if (bDrawTarget == true)
		target.Draw(program, CTM, g_time);

	glutSwapBuffers();
}

void myIdle()
{
	if (bPlay)
	{
		g_time += 1 / 60.0f;
		Sleep(1 / 60.0f * 1000);

		if (bChasingTarget == false)
		{
			ang1 = 45 * sin(g_time * 3.141592);
			ang2 = 60 * sin(g_time * 2 * 3.141592);
			ang3 = 30 * sin(g_time * 3.141592);
		}
		else
			computeAngle();

		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char c, int x, int y)
{

	switch (c)
	{
	case '1':
		bChasingTarget = !bChasingTarget;
		break;
	case '2':
		bDrawTarget = !bDrawTarget;
		break;
	case '3':
		target.toggleRandom();
		break;
	case ' ':
		bPlay = !bPlay;
		break;
	default:
		break;
	}
}

void PrintDesciption() {
	printf("SIMPLE INTELLIGENT ROBOT ARM\nProgramming Assignment #2 for Computer Graphics\n");
	printf("Department of Software, Sejong University\n\n");
	printf("----------------------------------------------------------------\n");
	printf("`1' key: Toggle Between Chasing the target or not\n");
	printf("`2' key : Toggle Between Showing the target position or not\n");
	printf("`3' key : Toggle Between Randomly Positioning the target or not\n\n");
	printf("Spacebar : start / stop Playing\n");
	printf("----------------------------------------------------------------\n\n");
}

int main(int argc, char** argv)
{
	PrintDesciption();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Simple Robot Arm");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}