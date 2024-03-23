#ifndef __MY_SPHERE_H__
#define __MY_SPHERE_H__
#include <vgl.h>
#include <vec.h>
#include <InitShader.h>

struct MySphereVertex
{
	vec4 pos;
	vec4 color;
};


class MySphere
{
public:
	int m_NumDiv;
	int m_NumVertex;

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_prog;

	bool m_bInit;

	MySphere()
	{
		m_NumDiv = 0;
		m_NumVertex = 0;
		m_bInit = false;
	}

	void init(int div)
	{
		if (m_NumDiv == div || div < 3) return;
		m_NumDiv = div;
		m_NumVertex = (m_NumDiv * 2 + (m_NumDiv - 2) * m_NumDiv * 2) * 3;
		//m_NumVertex = (m_NumDiv - 2) * m_NumDiv * 2 * 3;

		printf("cur div = %d  cur vertices = %d \n", m_NumDiv, m_NumVertex);

		MySphereVertex* vertices = new MySphereVertex[m_NumVertex];

		float r = 0.5f;

		int cur = 0;
		float da = 2 * 3.141592 / m_NumDiv;
		for (int i = 0; i < m_NumDiv; i++)
		{
			for (int j = 0; j < m_NumDiv; j++)
			{
				// (i,j), (i, j+1), (i+1, j+1), (i+1, j)
				//  a      b         c           d
				// i
				float z1 = cos(i * da);			// r cos(theta)
				float x1 = sin(i * da);			// r sin(theta)
				// i+1
				float z2 = cos((i + 1) * da);			// r cos(theta)
				float x2 = sin((i + 1) * da);			// r sin(theta)

				// j
				float j1 = 3.141592f / 2.0f - j * da / 2;
				float r1 = r * cos(j1);
				float y1 = r * sin(j1);

				// j+1
				float j2 = 3.141592f / 2.0f - (j + 1) * da / 2;
				float r2 = r * cos(j2);
				float y2 = r * sin(j2);

				vec4 a = vec4(r1 * x1, y1, r1 * z1, 1);		// (i,j)
				vec4 b = vec4(r2 * x1, y2, r2 * z1, 1);		// (i, j+1)
				vec4 c = vec4(r2 * x2, y2, r2 * z2, 1);		// (i+1, j+1)
				vec4 d = vec4(r1 * x2, y1, r1 * z2, 1);		// (i+1, j)

				vec4 color = vec4(1, 0, 0, 1);
				if ((i + j) % 2 == 1)
					color = vec4(0, 0, 1, 1);

				if (j != m_NumDiv - 1)
				{
					vertices[cur].pos = a;	vertices[cur].color = color; cur++;
					vertices[cur].pos = b;	vertices[cur].color = color; cur++;
					vertices[cur].pos = c;	vertices[cur].color = color; cur++;
				}

				if (j != 0)
				{
					vertices[cur].pos = c;	vertices[cur].color = color; cur++;
					vertices[cur].pos = d;	vertices[cur].color = color; cur++;
					vertices[cur].pos = a;	vertices[cur].color = color; cur++;
				}
			}
		}

		if (m_bInit == false)
		{
			// 1. vertex array object
			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);

			// 2. vertex buffer object
			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		}
		else
		{
			glBindVertexArray(m_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		}

		// 3. send to buffer data
		glBufferData(GL_ARRAY_BUFFER, sizeof(MySphereVertex) * m_NumVertex, vertices,
			GL_STATIC_DRAW);

		if (m_bInit == false)
			LoadShaderAndConnect();

		m_bInit = true;

		delete[] vertices;
	}
	void LoadShaderAndConnect()
	{
		// 4. load program
		m_prog = InitShader("vshader.glsl", "fshader.glsl");
		glUseProgram(m_prog);

		// 5. connect buffer data to shader attributes
		GLuint vPosition = glGetAttribLocation(m_prog, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE,
			sizeof(MySphereVertex), BUFFER_OFFSET(0));

		// vColor
		GLuint vColor = glGetAttribLocation(m_prog, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE,
			sizeof(MySphereVertex), BUFFER_OFFSET(sizeof(vec4)));
	}

	void decreasePoly()
	{
		init(m_NumDiv - 1);
	}
	void incresePoly()
	{
		init(m_NumDiv + 1);
	}


	void draw(float cur_time)
	{
		glBindVertexArray(m_vao);
		glUseProgram(m_prog);

		GLuint uTime = glGetUniformLocation(m_prog, "uTime");
		glUniform1f(uTime, cur_time);


		glDrawArrays(GL_TRIANGLES, 0, m_NumVertex);
	}



};


#endif