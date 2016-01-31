#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "OpenGL Shaders";

GLFWwindow *g_window = nullptr;

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    // Set up GLFW window and OpenGL settings
    glfwWindowHint(GLFW_SAMPLES, 4);                                // enable 4x anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // create 3.2 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // make GLFW forward-compatible (?)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // use OpenGL core profile

    g_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!g_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(1);                // vertical sync

    // Initialize GLEW with experimental driver support
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Set up shader programs
    GLuint program = LoadShaders("simple.vert", "simple.frag");
    glBindFragDataLocation(program, 0, "color_out");
    glUseProgram(program);

    GLuint vertArray;
    glCreateVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);
    
    // Set up VBO with vertex data
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    const GLfloat vertData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);

    // Set up VBO with color data
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    const GLfloat colorData[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

    // Specify layout of vertex data for shaders
    GLint positionAttr = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glVertexAttribPointer(positionAttr, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    GLint colorAttr = glGetAttribLocation(program, "color_in");
    glEnableVertexAttribArray(colorAttr);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(colorAttr, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // Draw
    while (!glfwWindowShouldClose(g_window)) {
        // Clear to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw vertices
        //glBindVertexArray(vertArray);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap buffers
        glfwSwapBuffers(g_window);

        // Process window events
        glfwPollEvents();
    }

    // Cleanup
    glDisableVertexAttribArray(colorAttr);
    glDisableVertexAttribArray(positionAttr);
    glDeleteBuffers(1, &vertBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteVertexArrays(1, &vertArray);
    glDeleteProgram(program);
    glfwDestroyWindow(g_window);
    glfwTerminate();

    return 0;
}
