#define _USE_MATH_DEFINES

#include "shaders.h"
#include <iostream>
#include <vector>
#include <cmath>

Task currentAssignment = Task2;

// шейдерные программы
GLuint ProgramQuad, ProgramFan, ProgramPentagon;
GLuint ProgramQuadUniform, ProgramFanUniform, ProgramPentagonUniform;
GLuint ProgramQuadGradient, ProgramFanGradient, ProgramPentagonGradient;

// атрибуты
GLint Attrib_vertex, Attrib_color;

// VBO
GLuint VBOQuad, VBOFan, VBOPentagon;
GLuint VBO_GradientQuad, VBO_GradientFan, VBO_GradientPentagon;

const char* VertexShaderSource = R"(
 #version 330 core
 in vec2 coord;
 void main() {
     gl_Position = vec4(coord, 0.0, 1.0);
 }
)";

const char* VertexColorShaderSource = R"(
 #version 330 core
 in vec2 coord;
 in vec4 vertColor;
 out vec4 fragColor;
 void main() {
     gl_Position = vec4(coord, 0.0, 1.0);
     fragColor = vertColor;
 }
)";

const char* FragShaderQuadSource = R"(
 #version 330 core
 out vec4 color;
 void main() { color = vec4(1, 0, 0, 1); }
)";

const char* FragShaderFanSource = R"(
 #version 330 core
 out vec4 color;
 void main() { color = vec4(0, 1, 0, 1); }
)";

const char* FragShaderPentagonSource = R"(
 #version 330 core
 out vec4 color;
 void main() { color = vec4(0, 0, 1, 1); }
)";

const char* FragShaderUniformSource = R"(
 #version 330 core
 out vec4 color;
 uniform vec4 uColor;
 void main() { color = uColor; }
)";

const char* FragShaderColorSource = R"(
 #version 330 core
 in vec4 fragColor;
 out vec4 color;
 void main() { color = fragColor; }
)";

void InitShaders() {
    // Vertex + Fragment для обычных фигур
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    glCompileShader(vShader);

    // Task2 Quad
    GLuint fQuadShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fQuadShader, 1, &FragShaderQuadSource, NULL);
    glCompileShader(fQuadShader);
    ProgramQuad = glCreateProgram();
    glAttachShader(ProgramQuad, vShader);
    glAttachShader(ProgramQuad, fQuadShader);
    glLinkProgram(ProgramQuad);

    // Task3 Uniform Quad
    GLuint fQuadUniform = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fQuadUniform, 1, &FragShaderUniformSource, NULL);
    glCompileShader(fQuadUniform);
    ProgramQuadUniform = glCreateProgram();
    glAttachShader(ProgramQuadUniform, vShader);
    glAttachShader(ProgramQuadUniform, fQuadUniform);
    glLinkProgram(ProgramQuadUniform);

    // Task4 Gradient
    GLuint vShaderColor = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShaderColor, 1, &VertexColorShaderSource, NULL);
    glCompileShader(vShaderColor);
    GLuint fShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderColor, 1, &FragShaderColorSource, NULL);
    glCompileShader(fShaderColor);

    ProgramQuadGradient = glCreateProgram();
    glAttachShader(ProgramQuadGradient, vShaderColor);
    glAttachShader(ProgramQuadGradient, fShaderColor);
    glLinkProgram(ProgramQuadGradient);

    ProgramFanGradient = glCreateProgram();
    glAttachShader(ProgramFanGradient, vShaderColor);
    glAttachShader(ProgramFanGradient, fShaderColor);
    glLinkProgram(ProgramFanGradient);

    ProgramPentagonGradient = glCreateProgram();
    glAttachShader(ProgramPentagonGradient, vShaderColor);
    glAttachShader(ProgramPentagonGradient, fShaderColor);
    glLinkProgram(ProgramPentagonGradient);

    // Task2 Fan
    GLuint fFanShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fFanShader, 1, &FragShaderFanSource, NULL);
    glCompileShader(fFanShader);
    ProgramFan = glCreateProgram();
    glAttachShader(ProgramFan, vShader);
    glAttachShader(ProgramFan, fFanShader);
    glLinkProgram(ProgramFan);

    // Task3 Uniform Fan
    GLuint fFanUniform = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fFanUniform, 1, &FragShaderUniformSource, NULL);
    glCompileShader(fFanUniform);
    ProgramFanUniform = glCreateProgram();
    glAttachShader(ProgramFanUniform, vShader);
    glAttachShader(ProgramFanUniform, fFanUniform);
    glLinkProgram(ProgramFanUniform);

    // Task2 Pentagon
    GLuint fPentagonShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fPentagonShader, 1, &FragShaderPentagonSource, NULL);
    glCompileShader(fPentagonShader);
    ProgramPentagon = glCreateProgram();
    glAttachShader(ProgramPentagon, vShader);
    glAttachShader(ProgramPentagon, fPentagonShader);
    glLinkProgram(ProgramPentagon);

    // Task3 Uniform Pentagon
    GLuint fPentagonUniform = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fPentagonUniform, 1, &FragShaderUniformSource, NULL);
    glCompileShader(fPentagonUniform);
    ProgramPentagonUniform = glCreateProgram();
    glAttachShader(ProgramPentagonUniform, vShader);
    glAttachShader(ProgramPentagonUniform, fPentagonUniform);
    glLinkProgram(ProgramPentagonUniform);
}

void InitVBO() {
    glGenBuffers(1, &VBOQuad);
    glGenBuffers(1, &VBOFan);
    glGenBuffers(1, &VBOPentagon);
    glGenBuffers(1, &VBO_GradientQuad);
    glGenBuffers(1, &VBO_GradientFan);
    glGenBuffers(1, &VBO_GradientPentagon);

    // Quad
    Vertex quad[6] = {
        {-0.95f,-0.3f}, {-0.95f,0.3f}, {-0.35f,0.3f}, // первый треугольник
        {-0.95f,-0.3f}, {-0.35f,0.3f}, {-0.35f,-0.3f}  // второй треугольник
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    ColorVertex quadGrad[4] = {
        {-0.95f,-0.3f,1,0,0,1}, {-0.95f,0.3f,0,1,0,1},
        {-0.35f,0.3f,0,0,1,1}, {-0.35f,-0.3f,1,1,0,1}
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadGrad), quadGrad, GL_STATIC_DRAW);

    // Fan
    //ColorVertex fanGrad[6];
    //float cx = 0.0f, cy = 0.0f, r = 0.3f;
    //fanGrad[0] = { cx,cy,1,1,1,1 };
    //for (int i = 1; i <= 5; i++) {
    //    float angle = M_PI * (i - 1) / 4.0f;
    //    fanGrad[i] = { cx + r * cos(angle),cy + r * sin(angle),float(i) / 5.0f,1.0f - float(i) / 5.0f,0.5f,1 };
    //}
    //glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientFan);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(fanGrad), fanGrad, GL_STATIC_DRAW);

    //Vertex fan[6]; fan[0] = { cx,cy };
    //for (int i = 1; i <= 5; i++) {
    //    float angle = M_PI * (i - 1) / 4.0f;
    //    fan[i] = { cx + r * cos(angle),cy + r * sin(angle) };
    //}
    //glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(fan), fan, GL_STATIC_DRAW);

    //Vertex fan[12];
    //float cx = 0.0f, cy = 0.0f, r = 0.3f;
    //Vertex center = { cx, cy };
    //for (int i = 0; i < 4; i++) {
    //    float angle1 = M_PI * i / 4.0f;
    //    float angle2 = M_PI * (i + 1) / 4.0f;
    //    fan[i * 3] = center;
    //    fan[i * 3 + 1] = { cx + r * cos(angle1), cy + r * sin(angle1) };
    //    fan[i * 3 + 2] = { cx + r * cos(angle2), cy + r * sin(angle2) };
    //}
    //glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(fan), fan, GL_STATIC_DRAW);

    {
        Vertex fan[12];
        float cx = 0.0f, cy = 0.0f, r = 0.3f;
        Vertex center = { cx, cy };
        for (int i = 0; i < 4; i++) {
            float angle1 = M_PI * i / 4.0f;
            float angle2 = M_PI * (i + 1) / 4.0f;
            fan[i * 3] = center;
            fan[i * 3 + 1] = { cx + r * cos(angle1), cy + r * sin(angle1) };
            fan[i * 3 + 2] = { cx + r * cos(angle2), cy + r * sin(angle2) };
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fan), fan, GL_STATIC_DRAW);

    }

    // Pentagon
    //ColorVertex pentGrad[5];
    //float px = 0.65f, py = 0.0f, pr = 0.3f;
    //for (int i = 0; i < 5; i++) {
    //    float angle = 2 * M_PI * i / 5.0 + M_PI / 2;
    //    pentGrad[i] = { px + pr * cos(angle),py + pr * sin(angle),float(i) / 5.0f,0.5f,float(i) / 5.0f,1 };
    //}
    //glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientPentagon);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(pentGrad), pentGrad, GL_STATIC_DRAW);

    //Vertex pent[5];
    //for (int i = 0; i < 5; i++) {
    //    float angle = 2 * M_PI * i / 5.0 + M_PI / 2;
    //    pent[i] = { px + pr * cos(angle),py + pr * sin(angle) };
    //}
    //glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(pent), pent, GL_STATIC_DRAW);

    {
        Vertex pent[9];
        float px = 0.65f, py = 0.0f, pr = 0.3f;
        Vertex pentCenter = { px, py };
        for (int i = 0; i < 3; i++) {
            float angle1 = 2 * M_PI * i / 5.0 + M_PI / 2;
            float angle2 = 2 * M_PI * (i + 1) / 5.0 + M_PI / 2;
            pent[i * 3] = pentCenter;
            pent[i * 3 + 1] = { px + pr * cos(angle1), py + pr * sin(angle1) };
            pent[i * 3 + 2] = { px + pr * cos(angle2), py + pr * sin(angle2) };
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pent), pent, GL_STATIC_DRAW);
    }
}

void Init() { 
    InitShaders(); 
    InitVBO(); 
}

void Draw() {
    if (currentAssignment == Task2) {
        // Quad (два треугольника)
        glUseProgram(ProgramQuad);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 вершин для двух треугольников
        glDisableVertexAttribArray(Attrib_vertex);

        // Fan (4 треугольника, 12 вершин)
        glUseProgram(ProgramFan);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 12); // 12 вершин для 4 треугольников
        glDisableVertexAttribArray(Attrib_vertex);

        // Pentagon (3 треугольника, 9 вершин)
        glUseProgram(ProgramPentagon);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 9); // 9 вершин для 3 треугольников
        glDisableVertexAttribArray(Attrib_vertex);
    }
    else if (currentAssignment == Task3) {
        // четырёхугольник (два треугольника)
        glUseProgram(ProgramQuadUniform);
        GLint colorLocQuad = glGetUniformLocation(ProgramQuadUniform, "uColor");
        glUniform4f(colorLocQuad, 0.0f, 1.0f, 0.0f, 1.0f);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(Attrib_vertex);
        glUseProgram(0);

        // веер (4 треугольника)
        glUseProgram(ProgramFanUniform);
        GLint colorLocFan = glGetUniformLocation(ProgramFanUniform, "uColor");
        glUniform4f(colorLocFan, 0.0f, 0.0f, 1.0f, 1.0f);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glDisableVertexAttribArray(Attrib_vertex);
        glUseProgram(0);

        // пятиугольник (3 треугольника)
        glUseProgram(ProgramPentagonUniform);
        GLint colorLocPentagon = glGetUniformLocation(ProgramPentagonUniform, "uColor");
        glUniform4f(colorLocPentagon, 1.0f, 0.0f, 0.0f, 1.0f);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        glDisableVertexAttribArray(Attrib_vertex);
        glUseProgram(0);
    }
    else if (currentAssignment == Task4) {
        // Gradient Quad
        glUseProgram(ProgramQuadGradient);
        Attrib_vertex = glGetAttribLocation(ProgramQuadGradient, "coord");
        Attrib_color = glGetAttribLocation(ProgramQuadGradient, "vertColor");
        glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientQuad);
        glEnableVertexAttribArray(Attrib_vertex);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)0);
        glEnableVertexAttribArray(Attrib_color);
        glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)(2 * sizeof(GLfloat)));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableVertexAttribArray(Attrib_vertex);
        glDisableVertexAttribArray(Attrib_color);

        // Gradient Fan
        glUseProgram(ProgramFanGradient);
        Attrib_vertex = glGetAttribLocation(ProgramFanGradient, "coord");
        Attrib_color = glGetAttribLocation(ProgramFanGradient, "vertColor");
        glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientFan);
        glEnableVertexAttribArray(Attrib_vertex);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)0);
        glEnableVertexAttribArray(Attrib_color);
        glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)(2 * sizeof(GLfloat)));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glDisableVertexAttribArray(Attrib_vertex);
        glDisableVertexAttribArray(Attrib_color);

        // Gradient Pentagon
        glUseProgram(ProgramPentagonGradient);
        Attrib_vertex = glGetAttribLocation(ProgramPentagonGradient, "coord");
        Attrib_color = glGetAttribLocation(ProgramPentagonGradient, "vertColor");
        glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientPentagon);
        glEnableVertexAttribArray(Attrib_vertex);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)0);
        glEnableVertexAttribArray(Attrib_color);
        glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)(2 * sizeof(GLfloat)));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
        glDisableVertexAttribArray(Attrib_vertex);
        glDisableVertexAttribArray(Attrib_color);
    }
}

void ReleaseVBO() {
    glDeleteBuffers(1, &VBOQuad); glDeleteBuffers(1, &VBOFan); glDeleteBuffers(1, &VBOPentagon);
    glDeleteBuffers(1, &VBO_GradientQuad); glDeleteBuffers(1, &VBO_GradientFan); glDeleteBuffers(1, &VBO_GradientPentagon);
}

void ReleaseShader() {
    glDeleteProgram(ProgramQuad); glDeleteProgram(ProgramFan); glDeleteProgram(ProgramPentagon);
    glDeleteProgram(ProgramQuadUniform); glDeleteProgram(ProgramFanUniform); glDeleteProgram(ProgramPentagonUniform);
    glDeleteProgram(ProgramQuadGradient); glDeleteProgram(ProgramFanGradient); glDeleteProgram(ProgramPentagonGradient);
}

void Release() { ReleaseShader(); ReleaseVBO(); }
