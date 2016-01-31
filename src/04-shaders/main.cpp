#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


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

    
    

}
