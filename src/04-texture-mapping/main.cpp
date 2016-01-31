#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;
static const char WINDOW_TITLE[] = "Texture Mapping";

static float aspectRatio = 3.0f / 2.0f;
static const float zOffset = 2.0f;

static float rotateY = 0.0f;
static float rotateX = 0.0f;

static const GLfloat g_vertexData[] = {
    -aspectRatio, -1.0f, zOffset,
     aspectRatio, -1.0f, zOffset,
     aspectRatio,  1.0f, zOffset,
    -aspectRatio, -1.0f, zOffset,
     aspectRatio,  1.0f, zOffset,
    -aspectRatio,  1.0f, zOffset
};

static const GLfloat g_uvData[] = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

GLFWwindow *g_window = nullptr;

static void OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(g_window, GL_TRUE);
            break;
        case GLFW_KEY_SPACE:
            rotateX = rotateY = 0.0f;
            break;
        case GLFW_KEY_Z:
            rotateX += 0.01f;
            break;
        case GLFW_KEY_X:
            rotateX -= 0.01f;
            break;
        case GLFW_KEY_A:
            rotateY += 0.01f;
            break;
        case GLFW_KEY_S:
            rotateY -= 0.01f;
            break;
    }
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);        // 4x anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!g_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(1);                // vertical sync

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(g_window, OnKeyPress);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint program = LoadShaders("transform.vert", "texture.frag");   
    char fileName[] = "panda.jpg";
    int imgWidth, imgHeight;
    GLuint textureId = LoadImageToTexture(fileName, &imgWidth, &imgHeight);

    aspectRatio = (float)imgWidth / imgHeight;
    if (!textureId) {
        std::cerr << "Failed to load image with texture" << std::endl;
        glfwTerminate();
        return 1;
    }

    GLuint MVPUniform = glGetUniformLocation(program, "MVP");
    GLuint textureSamplerUniform = glGetUniformLocation(program, "textureSampler");
    GLuint vertexAttr = glGetAttribLocation(program, "vertexPosition_modelspace");
    GLuint uvAttr = glGetAttribLocation(program, "vertexUV");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexData), g_vertexData, GL_STATIC_DRAW);
    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uvData), g_uvData, GL_STATIC_DRAW);

    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureSamplerUniform, 0);
    glEnableVertexAttribArray(vertexAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glVertexAttribPointer(vertexAttr, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(uvAttr);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(uvAttr, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);


    while (!glfwWindowShouldClose(g_window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        CalcMatrices(g_window);
        glm::mat4 modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::rotate(modelMatrix, glm::pi<float>() * rotateY, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::pi<float>() * rotateX, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 mvp = g_projMatrix * g_viewMatrix * modelMatrix;
        //glm::mat4 mvp = g_projMatrix;

        glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, &mvp[0][0]);

        //glPointSize(5.0f);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(g_window);

        glfwPollEvents();
    }

}