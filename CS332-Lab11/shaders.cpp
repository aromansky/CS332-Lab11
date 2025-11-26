#define _USE_MATH_DEFINES

#include "shaders.h"
#include <iostream>
#include <vector>
#include <cmath>

// это нужно для хранения программ шейдеров
GLuint ProgramQuad, ProgramFan, ProgramPentagon;

// это индекс атрибута для вершин
GLint Attrib_vertex;

// эта штука нужна для загрузки вершин в видеобуфер
GLuint VBOQuad, VBOFan, VBOPentagon;

const char* VertexShaderSource = R"(
 #version 330 core
 in vec2 coord;
 void main() {
 gl_Position = vec4(coord, 0.0, 1.0);
 }
)";

const char* FragShaderQuadSource = R"(
 #version 330 core
 out vec4 color;
 void main() {
 color = vec4(1, 0, 0, 1);
 }
)";

const char* FragShaderFanSource = R"(
 #version 330 core
 out vec4 color;
 void main() {
 color = vec4(0, 1, 0, 1);
 }
)";

const char* FragShaderPentagonSource = R"(
 #version 330 core
 out vec4 color;
 void main() {
 color = vec4(0, 0, 1, 1);
 }
)";

void InitShaders() {
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vShader);

	// четырёхугольник
	GLuint fQuadShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fQuadShader, 1, &FragShaderQuadSource, NULL);
	glCompileShader(fQuadShader);

	ProgramQuad = glCreateProgram();
	glAttachShader(ProgramQuad, vShader);
	glAttachShader(ProgramQuad, fQuadShader);
	glLinkProgram(ProgramQuad);

	// веер
	GLuint fFanShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fFanShader, 1, &FragShaderFanSource, NULL);
	glCompileShader(fFanShader);

	ProgramFan = glCreateProgram();
	glAttachShader(ProgramFan, vShader);
	glAttachShader(ProgramFan, fFanShader);
	glLinkProgram(ProgramFan);

	// пятиугольник
	GLuint fPentagonShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fPentagonShader, 1, &FragShaderPentagonSource, NULL);
	glCompileShader(fPentagonShader);

	ProgramPentagon = glCreateProgram();
	glAttachShader(ProgramPentagon, vShader);
	glAttachShader(ProgramPentagon, fPentagonShader);
	glLinkProgram(ProgramPentagon);
}

// загрука шейдеров вершин в буфер
void InitVBO() {
	glGenBuffers(1, &VBOQuad);
	glGenBuffers(1, &VBOFan);
	glGenBuffers(1, &VBOPentagon);

	// четырёхугольник
	Vertex quad[4] = {
		{-0.95f, -0.3f},
		{-0.95f, 0.3f},
		{-0.35f, 0.3f},
		{-0.35f, -0.3f}
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	// веер
	Vertex fan[6];
	float cx = -0.0f;
	float cy = 0.0f;
	float r = 0.3f;

	fan[0] = { cx, cy }; // центр
	for (int i = 1; i <= 5; i++) {
		float angle = M_PI * (i - 1) / 4.0f; // углы от 0 до 180 градусов
		fan[i] = { cx + r * cos(angle), cy + r * sin(angle) };
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan), fan, GL_STATIC_DRAW);

	// пятиугольник
	Vertex pentagon[5];
	float px = 0.65f;
	float py = 0.0f;
	float pr = 0.3f;

	for (int i = 0; i < 5; i++) {
		float angle = 2.0f * M_PI * i / 5.0f + M_PI / 2;
		pentagon[i] = { px + pr * cos(angle), py + pr * sin(angle) };
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pentagon), pentagon, GL_STATIC_DRAW);
}


void Init() {
	InitShaders();
	InitVBO();
}

void Draw() {
	// четырёхугольник
	glUseProgram(ProgramQuad);
	glEnableVertexAttribArray(Attrib_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
	glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(Attrib_vertex);
	glUseProgram(0);

	// веер
	glUseProgram(ProgramFan);
	glEnableVertexAttribArray(Attrib_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
	glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glDisableVertexAttribArray(Attrib_vertex);
	glUseProgram(0);

	// пятиугольник
	glUseProgram(ProgramPentagon);
	glEnableVertexAttribArray(Attrib_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
	glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
	glDisableVertexAttribArray(Attrib_vertex);
	glUseProgram(0);
}

void ReleaseVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBOQuad);
	glDeleteBuffers(1, &VBOFan);
	glDeleteBuffers(1, &VBOPentagon);
}

void ReleaseShader() {
	glUseProgram(0);
	glDeleteProgram(ProgramQuad);
	glDeleteProgram(ProgramFan);
	glDeleteProgram(ProgramPentagon);
}

void Release() {
	ReleaseShader();
	ReleaseVBO();
}