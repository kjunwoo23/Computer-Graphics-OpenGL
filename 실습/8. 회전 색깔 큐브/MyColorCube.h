#ifndef __MYCOLORCUBE_H__
#define __MYCOLORCUBE_H__
#include <vec.h>

class MyColorCube
{
public:

	int NumVertex;
	GLuint prog;
	GLuint vao;
	GLuint vbo;

	vec4* positions;
	vec4* colors;
	int cur;

	MyColorCube()
	{
		NumVertex = 36;				// =  6 * 2 * 3;
		cur = 0;
		positions = nullptr;
		colors = nullptr;
	}

	void makeRect(int a, int b, int c, int d, vec4 vertex_pos[], vec4 vertex_color[])
	{
		positions[cur] = vertex_pos[a];	colors[cur] = vertex_color[a]; cur++;
		positions[cur] = vertex_pos[b];	colors[cur] = vertex_color[b]; cur++;
		positions[cur] = vertex_pos[c];	colors[cur] = vertex_color[c]; cur++;

		positions[cur] = vertex_pos[c];	colors[cur] = vertex_color[c]; cur++;
		positions[cur] = vertex_pos[d];	colors[cur] = vertex_color[d]; cur++;
		positions[cur] = vertex_pos[a];	colors[cur] = vertex_color[a]; cur++;
	}

	void makeCube()
	{
		vec4 vertex_pos[8] = {
				vec4(-0.5,-0.5, 0.5, 1),
				vec4(-0.5, 0.5, 0.5, 1),
				vec4(0.5, 0.5, 0.5, 1),
				vec4(0.5,-0.5, 0.5, 1),
				vec4(-0.5,-0.5,-0.5, 1),
				vec4(-0.5, 0.5,-0.5, 1),
				vec4(0.5, 0.5, -0.5, 1),
				vec4(0.5,-0.5, -0.5, 1)
		};

		vec4 vertex_color[8] = {
			vec4(1,0,0,1),
			vec4(0,1,0,1),
			vec4(0,0,1,1),
			vec4(1,1,0,1),
			vec4(0,1,1,1),
			vec4(1,0,1,1),
			vec4(1,1,1,1),
			vec4(0,0,0,1)
		};

		cur = 0;

		makeRect(0, 3, 2, 1, vertex_pos, vertex_color);
		makeRect(4, 5, 6, 7, vertex_pos, vertex_color);
		makeRect(0, 4, 7, 3, vertex_pos, vertex_color);
		makeRect(3, 7, 6, 2, vertex_pos, vertex_color);
		makeRect(2, 6, 5, 1, vertex_pos, vertex_color);
		makeRect(1, 5, 4, 0, vertex_pos, vertex_color);
	}

	void init()
	{
		positions = new vec4[NumVertex];
		colors = new vec4[NumVertex];

		makeCube();

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

		glDrawArrays(GL_TRIANGLES, 0, NumVertex);
	}
};


#endif