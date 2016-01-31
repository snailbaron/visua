#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

// Triangle rotation speed, in revolutions per second
static const float ROTATE_SPPED_RPS = 0.1f;

int main()
{
    // Initialize GLFW library
    if (!glfwInit()) {
        return 1;
    }

    // Create a window with OpenGL context, and switch to the context
    GLFWwindow *window = glfwCreateWindow(800, 600, "Simple GLFW Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Render. The loop works until the window is closed.
    while (!glfwWindowShouldClose(window)) {
        // Get drawing area size, and calculate aspect ratio.
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspectRatio = (float)width / height;

        // Configure viewport for OpenGL drawing
        glViewport(0, 0, width, height);

        // Clear color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Set up camera matrix.
        // y takes values from [-1, 1].
        // x takes corresponding values (of the same scale), depending on width.
        // NOTE: Deprecated fixed-function pipeline is used for simplicity.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-aspectRatio, aspectRatio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw triangle with dynamic angle. The triangle is rotating thanks to glfwGetTime() call.
        // NOTE: Deprecated fixed-function pipeline.

        // Modify current model-view matrix by rotating around (0, 0, 1) axis.
        glRotatef((float)glfwGetTime() * 360.f * ROTATE_SPPED_RPS, 0.f, 0.f, 1.f);

        // Draw triangle
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();

        // Swap buffers to apply rendering changes
        glfwSwapBuffers(window);

        // Process window events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    // Release GLFW resources
    glfwTerminate();

    return 0;
}