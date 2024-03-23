#include <vgl.h>
#include <InitShader.h>
#include <queue>
#include "MyCube.h"
#include "MyUtil.h"

#include <vec.h>
#include <mat.h>

#define MAZE_FILE	"maze.txt"
#pragma warning (disable:4996)

//using namespace std;

MyCube cube;
GLuint program;

mat4 g_Mat = mat4(1.0f);
GLuint uMat;
GLuint uColor;

float wWidth = 1000;
float wHeight = 500;

vec3 cameraPos = vec3(0, 0, 0);
vec3 viewDirection = vec3(0, 0, -1);
vec3 goalPos = vec3(0, 0, 0);

int MazeSize;
char maze[255][255] = { 0 };

float cameraSpeed = 0.1;

float g_time = 0;

int rotate = 0;

bool leftLock, rightLock, upLock, downLock;
bool spacePressed;

vec2 startIdx;
vec2 goalIdx;
vec2 curIdx;

int costs[20][20] = { 0 };
bool shortCut[20][20];
bool qPressed;

bool autoLeft, autoRight, autoUp, autoDown;
bool autoTurnL, autoTurnR;
int autoCnt = 10;

vec2 velocity;

std::vector<vec2> path;


std::vector<vec2> pathFinding() {
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++) {
			costs[i][j] = INT_MAX;
			shortCut[i][j] = false;
		}
	int dx[] = { -1, 0, 1, 0 };
	int dy[] = { 0, 1, 0, -1 };
	std::priority_queue<std::pair<int, vec2>, std::vector<std::pair<int, vec2>>, std::greater<std::pair<int, vec2>>> pq;

	costs[(int)startIdx.x][(int)startIdx.y] = 0;
	pq.push({ 0, startIdx });

	while (!pq.empty()) {
		vec2 current = pq.top().second;
		int currentCost = pq.top().first;
		pq.pop();

		// 도착점에 도달하면 탐색 종료
		if (current.x == goalIdx.x && current.y == goalIdx.y) {
			break;
		}

		// 상하좌우 및 대각선 방향으로 이동
		for (int i = 0; i < 4; ++i) {
			int nx = (int)current.x + dx[i];
			int ny = (int)current.y + dy[i];

			// 미로 범위를 벗어나거나 벽인 경우 무시
			if (nx < 0 || ny < 0 || nx >= MazeSize || ny >= MazeSize || maze[nx][ny] == '*') {
				continue;
			}

			// 새로운 좌표까지의 비용 계산
			int newCost = currentCost + 1;

			// 더 작은 비용이면 업데이트하고 큐에 추가
			if (newCost < costs[nx][ny]) {
				costs[nx][ny] = newCost;
				pq.push({ newCost + abs(nx - (int)goalIdx.x) + abs(ny - (int)goalIdx.y), {(GLfloat)nx, (GLfloat)ny} });
			}
		}
	}

	// 최단경로 추적
	std::vector<vec2> path;
	vec2 current = goalIdx;

	while (!(current.x == startIdx.x && current.y == startIdx.y)) {
		path.push_back(current);
		int minCost = INT_MAX;
		int nextX = (int)current.x;
		int nextY = (int)current.y;

		// 현재 좌표에서 이동 가능한 좌표 중 최소 비용을 가진 좌표 선택
		for (int i = 0; i < 4; ++i) {
			int nx = current.x + dx[i];
			int ny = current.y + dy[i];

			if (nx >= 0 && ny >= 0 && nx < MazeSize && ny < MazeSize && costs[nx][ny] < minCost) {
				minCost = costs[nx][ny];
				nextX = nx;
				nextY = ny;
			}
		}

		current.x = nextX;
		current.y = nextY;
	}

	// 시작점 추가
	path.push_back(startIdx);

	// 경로를 역순으로 반환
	reverse(path.begin(), path.end());

	return path;
}


inline vec3 getPositionFromIndex(int i, int j)
{
	float unit = 1;
	vec3 leftTopPosition = vec3(-MazeSize / 2.0 + unit / 2, 0, -MazeSize / 2.0 + unit / 2);
	vec3 xDir = vec3(1, 0, 0);
	vec3 zDir = vec3(0, 0, 1);
	return leftTopPosition + i * xDir + j * zDir;
}

void LoadMaze()
{
	FILE* file = fopen(MAZE_FILE, "r");
	char buf[255];
	fgets(buf, 255, file);
	sscanf(buf, "%d", &MazeSize);
	for (int j = 0; j < MazeSize; j++)
	{
		fgets(buf, 255, file);
		for (int i = 0; i < MazeSize; i++)
		{
			maze[i][j] = buf[i];
			if (maze[i][j] == 'C')				// Setup Camera Position
			{
				cameraPos = getPositionFromIndex(i, j);
				startIdx = vec2(i, j);
				curIdx = startIdx;
			}
			if (maze[i][j] == 'G')				// Setup Goal Position
			{
				goalPos = getPositionFromIndex(i, j);
				goalIdx = vec2(i, j);
			}
		}
	}
	fclose(file);
}

void DrawMaze()
{
	for (int j = 0; j < MazeSize; j++)
		for (int i = 0; i < MazeSize; i++)
			if (maze[i][j] == '*')
			{
				vec3 color = vec3(i / (float)MazeSize, j / (float)MazeSize, 1);
				mat4 ModelMat = Translate(getPositionFromIndex(i, j));
				glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
				glUniform4f(uColor, color.x, color.y, color.z, 1);
				cube.Draw(program);
			}
}

void myInit()
{
	LoadMaze();
	cube.Init();
	program = InitShader("vshader.glsl", "fshader.glsl");

}

void DrawGrid()
{
	float n = 40;
	float w = MazeSize;
	float h = MazeSize;

	for (int i = 0; i < n; i++)
	{
		mat4 m = Translate(0, -0.5, -h / 2 + h / n * i) * Scale(w, 0.02, 0.02);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1, 1, 1, 1);
		cube.Draw(program);
	}
	for (int i = 0; i < n; i++)
	{
		mat4 m = Translate(-w / 2 + w / n * i, -0.5, 0) * Scale(0.02, 0.02, h);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1, 1, 1, 1);
		cube.Draw(program);
	}

	if (qPressed) {
		std::vector<vec2>::iterator p;
		float i;
		for (p = path.begin(), i = 0; p + 1 != path.end(); ++p, ++i) {
			shortCut[(int)(*p).x][(int)(*p).y] = true;
			vec3 matPos = (getPositionFromIndex((int)(*p).x, (int)(*p).y)
				+ getPositionFromIndex((int)(*(p + 1)).x, (int)(*(p + 1)).y))
				* 0.5 - vec3(0, 0.49, 0);
			mat4 m;
			if ((*p).x == (*(p + 1)).x)
				m = Translate(matPos) * Scale(0.1, 0.1, 1);
			else
				m = Translate(matPos) * Scale(1, 0.1, 0.1);
			glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
			glUniform4f(uColor, sin(i / 5 - g_time / 2), 0, 0, 1);
			cube.Draw(program);
		}
		shortCut[(int)goalIdx.x][(int)goalIdx.y] = true;
	}
}


void drawCamera()
{
	float cameraSize = 0.5;

	mat4 ModelMat = Translate(cameraPos) * RotateY(-rotate) * Scale(vec3(cameraSize));
	glUseProgram(program);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	if (spacePressed)	glUniform4f(uColor, 1, 0, 0, 1);
	else				glUniform4f(uColor, 0, 1, 0, 1);
	cube.Draw(program);

	ModelMat = Translate(cameraPos) * RotateY(-rotate) * Translate(viewDirection * cameraSize / 2) * Scale(vec3(cameraSize) / 2);
	glUseProgram(program);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	if (spacePressed)	glUniform4f(uColor, 1, 0, 0, 1);
	else				glUniform4f(uColor, 0, 1, 0, 1);
	cube.Draw(program);
}

void drawGoal()
{
	glUseProgram(program);
	float GoalSize = 0.7;

	mat4 ModelMat = Translate(goalPos) * RotateY(g_time * 3) * Scale(vec3(GoalSize));
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 0, 0, 0);
	cube.Draw(program);

	ModelMat = Translate(goalPos) * RotateY(g_time * 3 + 45) * Scale(vec3(GoalSize));
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 0, 0, 0);
	cube.Draw(program);
}


void drawScene(bool bDrawCamera = true)
{
	glUseProgram(program);
	uMat = glGetUniformLocation(program, "uMat");
	uColor = glGetUniformLocation(program, "uColor");

	DrawGrid();
	DrawMaze();
	drawGoal();

	if (bDrawCamera)
		drawCamera();
}

void display()
{
	glEnable(GL_DEPTH_TEST);

	float vWidth = wWidth / 2;
	float vHeight = wHeight;

	// LEFT SCREEN : View From Camera (Perspective Projection)
	glViewport(0, 0, vWidth, vHeight);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float h = 4;
	float aspectRatio = vWidth / vHeight;
	float w = aspectRatio * h;
	mat4 ViewMat = RotateY(rotate) * myLookAt(cameraPos, cameraPos + viewDirection, vec3(0, 1, 0));
	mat4 ProjMat = myPerspective(45, aspectRatio, 0.01, 20);

	g_Mat = ProjMat * ViewMat;
	drawScene(false);							// drawing scene excep the camera


	// RIGHT SCREEN : View from above (Orthographic parallel projection)
	glViewport(vWidth, 0, vWidth, vHeight);
	h = MazeSize;
	w = aspectRatio * h;
	ViewMat = myLookAt(vec3(0, 5, 0), vec3(0, 0, 0), vec3(0, 0, -1));
	ProjMat = myOrtho(-w / 2, w / 2, -h / 2, h / 2, 0, 20);

	g_Mat = ProjMat * ViewMat;
	drawScene(true);

	Sleep(16);
	glutSwapBuffers();
}

void moveLockCheck() {
	rightLock = false;
	leftLock = false;
	upLock = false;
	downLock = false;

	for (int i = 0; i < MazeSize; i++)
		for (int j = 0; j < MazeSize; j++) {
			vec3 wallPos;
			if (maze[i][j] == '*') {
				wallPos = getPositionFromIndex(i, j);
				if (-0.2 < cameraPos.z - wallPos.z + 0.4 && cameraPos.z - wallPos.z - 0.4 < 0.2) {
					if (-0.25 < cameraPos.x - wallPos.x + 0.5 && cameraPos.x - wallPos.x + 0.5 < 0)						rightLock = true;
					if (-0.25 < wallPos.x + 0.5 - cameraPos.x && wallPos.x + 0.5 - cameraPos.x < 0)						leftLock = true;
				}
				if (-0.2 < cameraPos.x - wallPos.x + 0.4 && cameraPos.x - wallPos.x - 0.4 < 0.2) {
					if (-0.25 < cameraPos.z - wallPos.z + 0.5 && cameraPos.z - wallPos.z + 0.5 < 0)						downLock = true;
					if (-0.25 < wallPos.z + 0.5 - cameraPos.z && wallPos.z + 0.5 - cameraPos.z < 0)						upLock = true;
				}
			}
		}

}

void myKeyboard(unsigned char c, int x, int y)
{
	if (c == ' ')
		if (spacePressed) spacePressed = false;
		else {
			spacePressed = true;
			autoCnt = 0;
			cameraPos = getPositionFromIndex((int)startIdx.x, (int)startIdx.y);
		}
	if (c == 'q' || c == 'Q') {
		//if (!qPressed) {
		qPressed = true;
		startIdx = curIdx;
		path = pathFinding();
	}
		//}
		/*else {
			path.clear();
			qPressed = false;
			spacePressed = false;
		}*/
}

void autoMove() {
	if ((int)curIdx.x == (int)goalIdx.x && (int)curIdx.y == (int)goalIdx.y) {
		cameraPos = getPositionFromIndex((int)goalIdx.x, (int)goalIdx.y);
		path.clear();
		qPressed = false;
		spacePressed = false;
		return;
	}
	//if (path.size() == 1) return;
	shortCut[(int)curIdx.x][(int)curIdx.y] = false;
	autoCnt--;

	if (rotate < 0) rotate += 360;

	std::vector<vec2>::iterator p = path.begin();

	if (autoCnt < 0) {
		if ((int)(*(p + 1)).x == (int)curIdx.x && (int)(*(p + 1)).y == (int)curIdx.y - 1) {
			if (rotate % 360 == 0) {
				autoUp = true;
				autoDown = false;
				autoLeft = false;
				autoRight = false;
				path.erase(path.begin());
				startIdx = *(path.begin());
				autoCnt = 10;
			}
			else if (rotate % 360 > 180)
				rotate += 9;
			else
				rotate -= 9;
		}
		else if ((int)(*(p + 1)).x == (int)curIdx.x && (int)(*(p + 1)).y == (int)curIdx.y + 1) {
			if (rotate % 360 == 180) {
				autoUp = false;
				autoDown = true;
				autoLeft = false;
				autoRight = false;
				path.erase(path.begin());
				startIdx = *(path.begin());
				autoCnt = 10;
			}
			else if (rotate % 360 < 180)
				rotate += 9;
			else
				rotate -= 9;
		}
		else if ((int)(*(p + 1)).x == (int)curIdx.x - 1 && (int)(*(p + 1)).y == (int)curIdx.y) {
			if (rotate % 360 == 270) {
				autoUp = false;
				autoDown = false;
				autoLeft = true;
				autoRight = false;
				path.erase(path.begin());
				startIdx = *(path.begin());
				autoCnt = 10;
			}
			else if (90 < rotate % 360 && rotate % 360 < 270)
				rotate += 9;
			else
				rotate -= 9;
		}
		else if ((int)(*(p + 1)).x == (int)curIdx.x + 1 && (int)(*(p + 1)).y == (int)curIdx.y) {
			if (rotate % 360 == 90) {
				autoUp = false;
				autoDown = false;
				autoLeft = false;
				autoRight = true;
				path.erase(path.begin());
				startIdx = *(path.begin());
				autoCnt = 10;
			}
			else if (90 < rotate % 360 && rotate % 360 < 270)
				rotate -= 9;
			else
				rotate += 9;
		}
	}

	else {
		if (autoLeft)
			cameraPos += cameraSpeed * vec3(-1, 0, 0);

		if (autoRight)
			cameraPos += cameraSpeed * vec3(1, 0, 0);

		if (autoUp)
			cameraPos += cameraSpeed * vec3(0, 0, -1);

		if (autoDown)
			cameraPos += cameraSpeed * vec3(0, 0, 1);
	}
}


void idle()
{
	g_time += 1;
	moveLockCheck();

	curIdx.x = (int)(cameraPos - vec3(-MazeSize / 2.0 + 1 / 2, 0, -MazeSize / 2.0 + 1 / 2)).x;
	curIdx.y = (int)(cameraPos - vec3(-MazeSize / 2.0 + 1 / 2, 0, -MazeSize / 2.0 + 1 / 2)).z;

	if (spacePressed && !path.empty())
		autoMove();
	else
	{		
		if ((GetAsyncKeyState('W') & 0x8000) == 0x8000)		// if "W" key is pressed	: Go Forward
		{
			velocity.x = sin(rotate * 3.141592 / 180);
			velocity.y = -cos(rotate * 3.141592 / 180);

			if (leftLock && velocity.x < 0) velocity.x = 0;
			if (rightLock && velocity.x > 0) velocity.x = 0;
			if (upLock && velocity.y < 0) velocity.y = 0;
			if (downLock && velocity.y > 0) velocity.y = 0;

			cameraPos += cameraSpeed * vec3(velocity.x, 0, velocity.y);
		}
		if ((GetAsyncKeyState('S') & 0x8000) == 0x8000)		// if "S" key is pressed	: Go Backward
		{
			velocity.x = -sin(rotate * 3.141592 / 180);
			velocity.y = cos(rotate * 3.141592 / 180);

			if (leftLock && velocity.x < 0) velocity.x = 0;
			if (rightLock && velocity.x > 0) velocity.x = 0;
			if (upLock && velocity.y < 0) velocity.y = 0;
			if (downLock && velocity.y > 0) velocity.y = 0;

			cameraPos += cameraSpeed * vec3(velocity.x, 0, velocity.y);
		}

		if ((GetAsyncKeyState('A') & 0x8000) == 0x8000)		// if "Q" key is pressed	: 
			rotate -= 9;
		if ((GetAsyncKeyState('D') & 0x8000) == 0x8000)		// if "E" key is pressed	: 
			rotate += 9;
	}

	Sleep(16);											// for vSync
	glutPostRedisplay();
}

void reshape(int wx, int wy)
{
	printf("%d %d \n", wx, wy);
	wWidth = wx;
	wHeight = wy;
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(wWidth, wHeight);

	glutCreateWindow("Homework3 (Maze Navigator)");

	glewExperimental = true;
	glewInit();

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	myInit();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(myKeyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}