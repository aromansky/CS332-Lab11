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

    // ---------- QUAD (2 triangles = 6 vertices) ----------
    Vertex quad[6] = {
        {-0.95f,-0.3f}, {-0.95f, 0.3f}, {-0.35f, 0.3f},
        {-0.95f,-0.3f}, {-0.35f, 0.3f}, {-0.35f,-0.3f}
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    ColorVertex quadGrad[6] = {
        {-0.95f,-0.3f,1,0,0,1}, {-0.95f,0.3f,0,1,0,1}, {-0.35f,0.3f,0,0,1,1},
        {-0.95f,-0.3f,1,0,0,1}, {-0.35f,0.3f,0,0,1,1}, {-0.35f,-0.3f,1,1,0,1}
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadGrad), quadGrad, GL_STATIC_DRAW);


    // ---------- FAN (center + 5 points → 4 triangles = 12 vertices) ----------
    float cx = 0.0f, cy = 0.0f, r = 0.3f;
    Vertex fan[6];
    fan[0] = { cx, cy }; // center
    for (int i = 1; i <= 5; i++) {
        float angle = M_PI * (i - 1) / 4.0f;
        fan[i] = { cx + r * cos(angle), cy + r * sin(angle) };
    }

    // convert FAN → TRIANGLES
    Vertex fanTri[12] = {
        fan[0], fan[1], fan[2],
        fan[0], fan[2], fan[3],
        fan[0], fan[3], fan[4],
        fan[0], fan[4], fan[5]
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fanTri), fanTri, GL_STATIC_DRAW);

    // gradient FAN
    ColorVertex fanG[6];
    fanG[0] = { cx,cy,1,1,1,1 };
    for (int i = 1; i <= 5; i++) {
        float angle = M_PI * (i - 1) / 4.0f;
        fanG[i] = { cx + r * cos(angle), cy + r * sin(angle), float(i) / 5, 1.0f - float(i) / 5, 0.5f, 1 };
    }

    ColorVertex fanGtri[12] = {
        fanG[0], fanG[1], fanG[2],
        fanG[0], fanG[2], fanG[3],
        fanG[0], fanG[3], fanG[4],
        fanG[0], fanG[4], fanG[5]
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientFan);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fanGtri), fanGtri, GL_STATIC_DRAW);


    // ---------- PENTAGON (5 points → 3 triangles = 9 vertices) ----------
    float px = 0.65f, py = 0.0f, pr = 0.3f;
    Vertex pent[5];
    for (int i = 0; i < 5; i++) {
        float angle = 2 * M_PI * i / 5 + M_PI / 2;
        pent[i] = { px + pr * cos(angle), py + pr * sin(angle) };
    }

    Vertex pentTri[9] = {
        pent[0], pent[1], pent[2],
        pent[0], pent[2], pent[3],
        pent[0], pent[3], pent[4]
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pentTri), pentTri, GL_STATIC_DRAW);

    // gradient pentagon
    ColorVertex pentG[5];
    for (int i = 0; i < 5; i++) {
        float angle = 2 * M_PI * i / 5 + M_PI / 2;
        pentG[i] = { px + pr * cos(angle), py + pr * sin(angle), float(i) / 5, 0.5f, float(i) / 5, 1 };
    }

    ColorVertex pentGtri[9] = {
        pentG[0], pentG[1], pentG[2],
        pentG[0], pentG[2], pentG[3],
        pentG[0], pentG[3], pentG[4]
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientPentagon);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pentGtri), pentGtri, GL_STATIC_DRAW);
}


void Init() {
    InitShaders();
    InitVBO();
}

void Draw() {
    if (currentAssignment == Task2) {

        // QUAD (6 vertices)
        glUseProgram(ProgramQuad);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(Attrib_vertex);

        // FAN (12 vertices)
        glUseProgram(ProgramFan);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glDisableVertexAttribArray(Attrib_vertex);

        // PENTAGON (9 vertices)
        glUseProgram(ProgramPentagon);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        glDisableVertexAttribArray(Attrib_vertex);
    }

    else if (currentAssignment == Task3) {

        // QUAD
        glUseProgram(ProgramQuadUniform);
        GLint colorLocQuad = glGetUniformLocation(ProgramQuadUniform, "uColor");
        glUniform4f(colorLocQuad, 0, 1, 0, 1);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(Attrib_vertex);

        // FAN
        glUseProgram(ProgramFanUniform);
        GLint colorLocFan = glGetUniformLocation(ProgramFanUniform, "uColor");
        glUniform4f(colorLocFan, 0, 0, 1, 1);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOFan);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glDisableVertexAttribArray(Attrib_vertex);

        // PENTAGON
        glUseProgram(ProgramPentagonUniform);
        GLint colorLocPent = glGetUniformLocation(ProgramPentagonUniform, "uColor");
        glUniform4f(colorLocPent, 1, 0, 0, 1);
        glEnableVertexAttribArray(Attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPentagon);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        glDisableVertexAttribArray(Attrib_vertex);
    }

    else if (currentAssignment == Task4) {

        // ---- Gradient QUAD ----
        glUseProgram(ProgramQuadGradient);
        Attrib_vertex = glGetAttribLocation(ProgramQuadGradient, "coord");
        Attrib_color = glGetAttribLocation(ProgramQuadGradient, "vertColor");
        glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientQuad);
        glEnableVertexAttribArray(Attrib_vertex);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE,
            sizeof(ColorVertex), (void*)0);
        glEnableVertexAttribArray(Attrib_color);
        glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE,
            sizeof(ColorVertex), (void*)(2 * sizeof(GLfloat)));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(Attrib_vertex);
        glDisableVertexAttribArray(Attrib_color);

        // ---- Gradient FAN ----
        glUseProgram(ProgramFanGradient);
        Attrib_vertex = glGetAttribLocation(ProgramFanGradient, "coord");
        Attrib_color = glGetAttribLocation(ProgramFanGradient, "vertColor");
        glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientFan);
        glEnableVertexAttribArray(Attrib_vertex);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE,
            sizeof(ColorVertex), (void*)0);
        glEnableVertexAttribArray(Attrib_color);
        glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE,
            sizeof(ColorVertex), (void*)(2 * sizeof(GLfloat)));
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glDisableVertexAttribArray(Attrib_vertex);
        glDisableVertexAttribArray(Attrib_color);

        // ---- Gradient PENTAGON ----
        glUseProgram(ProgramPentagonGradient);
        Attrib_vertex = glGetAttribLocation(ProgramPentagonGradient, "coord");
        Attrib_color = glGetAttribLocation(ProgramPentagonGradient, "vertColor");
        glBindBuffer(GL_ARRAY_BUFFER, VBO_GradientPentagon);
        glEnableVertexAttribArray(Attrib_vertex);
        glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE,
            sizeof(ColorVertex), (void*)0);
        glEnableVertexAttribArray(Attrib_color);
        glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE,
            sizeof(ColorVertex), (void*)(2 * sizeof(GLfloat)));
        glDrawArrays(GL_TRIANGLES, 0, 9);
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