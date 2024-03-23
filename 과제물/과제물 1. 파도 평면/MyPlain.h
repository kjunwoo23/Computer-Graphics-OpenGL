#ifndef __MYPLAIN_H__
#define __MYPLAIN_H__
#include <vec.h>

class MyPlain
{
public:

	int NumVertex;

	float plainSize;
	int rowNum;
	bool wave;

	GLuint prog;
	GLuint vao;
	GLuint vbo;

	vec4* positions;
	vec4* colors;
	int cur;

	MyPlain()
	{
		rowNum = 30;
		plainSize = 0.75f;
		NumVertex = 6 * (int)pow(rowNum, 2);
		cur = 0;
		positions = nullptr;
		colors = nullptr;
	}

	void makeRect(int a, int b, int c, int d, vec4 vertex_pos[], vec4 vertex_color)
	{
		positions[cur] = vertex_pos[a];	colors[cur] = vertex_color; cur++;
		positions[cur] = vertex_pos[b];	colors[cur] = vertex_color; cur++;
		positions[cur] = vertex_pos[c];	colors[cur] = vertex_color; cur++;

		positions[cur] = vertex_pos[c];	colors[cur] = vertex_color; cur++;
		positions[cur] = vertex_pos[d];	colors[cur] = vertex_color; cur++;
		positions[cur] = vertex_pos[a];	colors[cur] = vertex_color; cur++;
	}

	void makePlain(vec4 v1, vec4 v2, bool bold)
	{
		vec4 vertex_pos[4] = {
			vec4(v2.x, 0, v1.z, 1),
			vec4(v1.x, 0, v1.z, 1),
			vec4(v2.x, 0,v2.z, 1),
			vec4(v1.x, 0, v2.z, 1),
		};

		float c = 0;
		if (bold) c = 0.6f;
		else c = 0.5f;

		vec4 vertex_color = vec4(c, c, c, 1);

		makeRect(1, 3, 2, 0, vertex_pos, vertex_color);
	}

	void init()
	{
		positions = new vec4[NumVertex];
		colors = new vec4[NumVertex];

		cur = 0;
		for (int i = 0; i < rowNum; i++)
			for (int j = 0; j < rowNum; j++) {
				float leftDownCornerX = (-plainSize + 2 * (plainSize / rowNum) * j);
				float leftDownCornerZ = (-plainSize + 2 * (plainSize / rowNum) * i);

				bool bold;
				if (abs(j - i) % 2 == 0) bold = false;
				else bold = true;
				makePlain(vec4(leftDownCornerX + 2 * plainSize / rowNum, 0, leftDownCornerZ + 2 * plainSize / rowNum, 1), vec4(leftDownCornerX, 0, leftDownCornerZ, 1), bold);
			}

		//1. Setup VAO (gen and bind)

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//2. Setup VBO

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//3. Copy data to Buffer

		glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * NumVertex * 2, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * NumVertex, positions);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * NumVertex, sizeof(vec4) * NumVertex, colors);

		//4. Load Shaders

		prog = InitShader("vshader.glsl", "fshader.glsl");
		glUseProgram(prog);

		GLuint vPosition = glGetAttribLocation(prog, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// vColor
		GLuint vColor = glGetAttribLocation(prog, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * NumVertex));


		delete[] positions;
		delete[] colors;

	}

	void draw(float cur_time)
	{
		glBindVertexArray(vao);
		glUseProgram(prog);

		GLuint uTime = glGetUniformLocation(prog, "uTime");
		glUniform1f(uTime, cur_time);

		GLuint uPlainSize = glGetUniformLocation(prog, "uPlainSize");
		glUniform1f(uPlainSize, plainSize);

		GLuint uWave = glGetUniformLocation(prog, "uWave");
		glUniform1i(uWave, wave);
		glDrawArrays(GL_TRIANGLES, 0, NumVertex);
	}
};


#endif