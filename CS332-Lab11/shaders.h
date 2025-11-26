#pragma once

#include <GL/glew.h>
#include <vector>

extern enum Task {
    Task2 = 2,
	Task3 = 3,
	Task4 = 4
};

extern Task currentAssignment;

// это нужно для хранения программ шейдеров
extern GLuint ProgramQuad, ProgramFan, ProgramPentagon;
extern GLuint ProgramQuadUniform, ProgramFanUniform, ProgramPentagonUniform;
extern GLuint ProgramQuadGradient, ProgramFanGradient, ProgramPentagonGradient;

// это индекс атрибута для вершин
extern GLint Attrib_vertex, Attrib_color;

// эта штука нужна для загрузки вершин в видеобуфер
extern GLuint VBOQuad, VBOFan, VBOPentagon;
extern GLuint VBO_Gradient;

extern const char* VertexShaderSource;

extern const char* FragShaderQuadSource;
extern const char* FragShaderFanSource;
extern const char* FragShaderPentagonSource;

extern const char* FragShaderUniformSource;

extern const char* FragShaderQuadSource;

// структура для хранения координат вершин
struct Vertex {
	GLfloat x;
	GLfloat y;
};

struct ColorVertex {
    GLfloat x;
    GLfloat y;
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

void InitShaders();

void InitVBO();

void Init();

void Draw();

void Release();