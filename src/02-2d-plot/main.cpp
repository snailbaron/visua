#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <cmath>

struct Color
{
    GLfloat r, g, b, a;
};



static const GLfloat AXIS_WIDTH = 2.0f;
static const Color AXIS_COLOR = { 1.f, 1.f, 0.5f, 1.f };

static const Color DATA_POINT_COLOR = { 1.f, 1.f, 1.f, 1.f };
static const Color DATA_LINE_COLOR = { 0.5f, 1.0f, 0.5f, 1.0f };
static const GLfloat DATA_POINT_SIZE = 4.f;
static const GLfloat CHART_WIDTH = 1.2f;

static const GLfloat GRID_DELTA = 0.2f;
static const Color GRID_COLOR = { 0.5f, 0.5f, 0.5f, 1.f };
static const GLfloat GRID_LINE_WIDTH = 1.f;

struct Point
{
    GLfloat x, y;
};

void DrawPoint(const Point &p, const Color &c)
{
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    
    glPointSize(DATA_POINT_SIZE);
    glBegin(GL_POINTS);
    glColor4f(c.r, c.g, c.b, c.a);
    glVertex3f(p.x, p.y, 0.f);
    glEnd();
}

void DrawLineSegment(const Point &p1, const Point &p2, const Color &color, GLfloat width)
{
    glLineWidth(width);
    glBegin(GL_LINES);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex3f(p1.x, p1.y, 0.f);
    glVertex3f(p2.x, p2.y, 0.f);
    glEnd();
}

void DrawAxes()
{
    Point xp1 = { -10.f, 0.f }, xp2 = { 10.f, 0.f };
    DrawLineSegment(xp1, xp2, AXIS_COLOR, AXIS_WIDTH);
    Point yp1 = { 0.f, -1.f }, yp2 = { 0.f, 1.f };
    DrawLineSegment(yp1, yp2, AXIS_COLOR, AXIS_WIDTH);
}

void DrawGrid()
{
    for (GLfloat x = -2.0f; x <= 2.0f; x += GRID_DELTA) {
        Point p1 = { x, -1.0f }, p2 = { x, 1.0f };
        DrawLineSegment(p1, p2, GRID_COLOR, GRID_LINE_WIDTH);
    }
    for (GLfloat y = -2.0f; y <= 2.0f; y += GRID_DELTA) {
        Point p1 = { -2.0f, y }, p2 = { 2.0f, y };
        DrawLineSegment(p1, p2, GRID_COLOR, GRID_LINE_WIDTH);
    }
}

void DrawScatterPlot(const Point *points, int n)
{
    for (int i = 0; i < n; i++) {
        DrawPoint(points[i], DATA_POINT_COLOR);
    }
}

void DrawPlot(const Point *points, int n)
{
    for (int i = 0; i < n-1; i++) {
        DrawLineSegment(points[i], points[i+1], DATA_LINE_COLOR, CHART_WIDTH);
    }
}

// Draw a demo plot
void Demo(GLfloat phase)
{
    DrawGrid();
    DrawAxes();

    const GLfloat ampl = 0.8f;
    const GLfloat freq = 1.0f;
    const GLfloat range = 10.f;
    const int n = 200;
    Point *points = new Point[n];
    for (int i = 0; i < n; i++) {
        points[i].x = ((GLfloat)i / (n-1)) * range - range / 2;
        points[i].y = ampl * cosf(points[i].x * (GLfloat)M_PI * freq + phase);
    }
    DrawScatterPlot(points, n);
    DrawPlot(points, n);
    delete[] points;
}

int main()
{
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "2D Plot", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    float phase = 0.f;
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        phase += 0.02f;
        Demo(phase);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}