#include <GLFW/glfw3.h>

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

struct Vertex
{
    GLfloat x, y, z;        // vertex location
    GLfloat r, g, b, a;     // vertex color with alpha channel
};

void DrawPoint(const Vertex &vertex, GLfloat size)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    glColor4f(vertex.r, vertex.g, vertex.b, vertex.a);
    glVertex3f(vertex.x, vertex.y, vertex.z);
    glEnd();
}

void DrawLineSegment(const Vertex &v1, const Vertex &v2, GLfloat width)
{
    glLineWidth(width);
    glBegin(GL_LINES);
    glColor4f(v1.r, v1.g, v1.b, v1.a);
    glVertex3f(v1.x, v1.y, v1.z);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(v2.x, v2.y, v2.z);
    glEnd();
}

void DrawTirangle(const Vertex &v1, const Vertex &v2, const Vertex &v3)
{
    const Vertex *vs[3] = { &v1, &v2, &v3 };

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++) {
        glColor4f(vs[i]->r, vs[i]->g, vs[i]->b, vs[i]->a);
        glVertex3f(vs[i]->x, vs[i]->y, vs[i]->z);
    }
    glEnd();
}

// Rendering function for points drawing
void RenderPoints()
{
    for (int i = 0; i < 7; i++) {
        GLfloat size = 3.f + i * 1.5f;
        GLfloat x = -0.5f + i * 0.2f;
        Vertex v = {
            x, 0.f, 0.f,
            1.f, 1.f, 1.f, 1.f
        };
        DrawPoint(v, size);
    }
}

void RenderGrid(GLfloat width, GLfloat height, GLfloat delta)
{
    // Horizontal lines
    for (float i = -height; i < height; i += delta) {
        Vertex v1 = { -width, i, 0.f, 1.f, 1.f, 1.f, 1.f };
        Vertex v2 = { width, i, 0.f, 1.f, 1.f, 1.f, 1.f };
        DrawLineSegment(v1, v2, 1.f);
    }

    // Vertical lines
    for (float i = -width; i < width; i += delta) {
        Vertex v1 = { i, -height, 0.f, 1.f, 1.f, 1.f, 1.f };
        Vertex v2 = { i, height, 0.f, 1.f, 1.f, 1.f, 1.f };
        DrawLineSegment(v1, v2, 1.f);
    }
}

void RenderLineSegments()
{
    RenderGrid(5.f, 1.f, 0.1f);
    Vertex v1 = { -5.f,  0.f, 0.f, 1.f, 0.f, 0.f, 0.7f };
    Vertex v2 = {  5.f,  0.f, 0.f, 0.f, 1.f, 0.f, 0.7f };
    Vertex v3 = {  0.f,  1.f, 0.f, 0.f, 0.f, 1.f, 0.7f };
    Vertex v4 = {  0.f, -1.f, 0.f, 0.f, 0.f, 1.f, 0.7f };
    DrawLineSegment(v1, v2, 10.f);
    DrawLineSegment(v3, v4, 10.f);
}

void RenderTriangle()
{
    RenderGrid(5.f, 1.f, 0.1f);
    Vertex v1 = { -1.f,  -0.5f, 0.f, 1.f, 0.f, 0.f, 0.7f };
    Vertex v2 = {  1.f,  -0.5f, 0.f, 0.f, 1.f, 0.f, 0.7f };
    Vertex v3 = {  0.f,  0.8f, 0.f, 0.f, 0.f, 1.f, 0.7f };
    DrawTirangle(v1, v2, v3);
}

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        return 1;
    }

    // Create window with OpenGL context, and use it
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Primitives", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Enable point smoothing (render them as "circles")
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    // Enable blending (anti-aliasing)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render while the OpenGL window is active
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = (float)width / height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set up projection matrix to simple orthographic projection.
        // y is in [-1, 1] range, x is in according 0-simmetrical interval.
        // x axis is left to right, y is bottom to top, z is from screen to viewer.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Perform rendering
        int timeFrame = (int)glfwGetTime() / 2;
        if (timeFrame % 3 == 0) {
            RenderPoints();
        } else if (timeFrame % 3 == 1) {
            RenderLineSegments();
        } else {
            RenderTriangle();
        }

        // Swap buffers to see rendering effect
        glfwSwapBuffers(window);

        // Process window messages
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}