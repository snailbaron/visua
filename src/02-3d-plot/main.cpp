#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

GLfloat alpha = 210.f, beta = -70.0f, zoom = 2.0f;

struct Point3D
{
    GLfloat x, y, z;
    Point3D() : x(0.0f), y(0.0f), z(0.0f) {}
    Point3D(GLfloat ax, GLfloat ay, GLfloat az) : x(ax), y(ay), z(az) {}
};

class Parabola
{
public:
    Parabola(float x0, float y0, float a) : m_x0(x0), m_y0(y0), m_a(a) {}
    ~Parabola() {}

    float A() const { return m_a; }
    Parabola & SetA(float a) { m_a = a; return *this; }

    float operator()(float x, float y) {
        return m_a * ((x - m_x0) * (x - m_x0) + (y - m_y0) * (y - m_y0));
    }

private:
    float m_x0, m_y0;
    float m_a;
};

class Gaussian
{
public:
    Gaussian(float x0, float y0, float sigmaX, float sigmaY) :
        m_x0(x0), m_y0(y0), m_sigmaX(sigmaX), m_sigmaY(sigmaY) {}
    ~Gaussian() {}

    float SigmaX() const { return m_sigmaX; }
    float SigmaY() const { return m_sigmaY; }
    Gaussian & SetSigmaX(float sigmaX) { m_sigmaX = sigmaX; return *this; }
    Gaussian & SetSigmaY(float sigmaY) { m_sigmaY = sigmaY; return *this; }

    float operator()(float x, float y) {
        float A = 1.0f / (2.0f * (float)M_PI * m_sigmaX * m_sigmaY);
        float e = exp(- 0.5f * (x - m_x0) * (x - m_x0) / (m_sigmaX * m_sigmaX) - 0.5f * (y - m_y0) * (y - m_y0) / (m_sigmaY * m_sigmaY));
        //return A * e;
        return e;
    }

private:
    float m_x0, m_y0;
    float m_sigmaX, m_sigmaY;
};

void OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS) {
        return;
    }

    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_LEFT:
            alpha += 5.0f;
            break;

        case GLFW_KEY_RIGHT:
            alpha -= 5.0f;
            break;
    }
}

static bool g_locked = false;

void OnMouseButtonPress(GLFWwindow *window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT) {
        return;
    }

    if (action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        g_locked = true;
    } else { 
        g_locked = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

GLfloat g_cursorX = 0.0f, g_cursorY = 0.0f;

void OnMouseMove(GLFWwindow *window, double x, double y)
{
    if (g_locked) {
        alpha += (GLfloat)(x - g_cursorX) / 10.0f;
        beta += (GLfloat)(y - g_cursorY) / 10.0f;
    }
    g_cursorX = (GLfloat)x;
    g_cursorY = (GLfloat)y;
}

void OnWindowResize(GLFWwindow *window, int width, int height)
{
    assert(width > 0 && height > 0);
    const float frontPlane = 0.1f;
    const float backPlane = 128.0f;

    float ratio = (float)width / height;
    float fovY = (float)M_PI_4;
    float tangent = tanf(fovY / 2);
    float h = frontPlane * tangent;
    float w = h * ratio;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-w, w, -h, h, frontPlane, backPlane);
    glTranslatef(0.0f, 0.0f, -2.0f);
}

int main()
{
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "3D Plot", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);        // vertical synchronization?

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    OnWindowResize(window, width, height);

    glfwSetKeyCallback(window, OnKeyPress);
    glfwSetMouseButtonCallback(window, OnMouseButtonPress);
    glfwSetCursorPosCallback(window, OnMouseMove);

    // Enable anti-aliasing and smoothing for points and lines
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);

    Parabola parabola(0.0f, 0.0f, 0.5f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // Set model-view matrix for chart
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -2.0f);
        glRotatef(beta, 1.0f, 0.0f, 0.0f);      // rotate by beta around x-axis
        glRotatef(alpha, 0.0f, 0.0f, 1.0f);     // rotate by alpha around z-axis

        // Draw origin and axes
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
        glLineWidth(4.0f);
        glBegin(GL_LINES);
        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.3f, 0.0f, 0.0f);
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.3f);
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.3f, 0.0f);
        glEnd();

        // Modify Gaussian over time
        //if (gauss.SigmaX() <= minSigma) {
        //    gauss.SetSigmaX(minSigma).SetSigmaY(minSigma);
        //    direction = 1;
        //} else if (gauss.SigmaX() >= maxSigma) {
        //    gauss.SetSigmaX(maxSigma).SetSigmaY(maxSigma);
        //    direction = -1;
        //}
        //gauss.SetSigmaX(gauss.SigmaX() + direction * sigmaDelta);
        //gauss.SetSigmaY(gauss.SigmaY() + direction * sigmaDelta);
        float newA = parabola.A();
        if (newA > 1.0f) {
            newA = 0.01f;
        }
        newA += 0.01f;
        parabola.SetA(newA);

        // Calculate data for parabola chart
        const int xn = 100, yn = 100;
        std::vector<Point3D> points(xn * yn);
        for (int i = 0; i < xn; i++) {
            for (int j = 0; j < yn; j++) {
                int k = i * yn + j;
                points[k].x = i * 2.0f / (xn - 1) - 1.0f;
                points[k].y = j * 2.0f / (yn - 1) - 1.0f;
                points[k].z = parabola(points[k].x, points[k].y);
            }
        }
        auto minPoint = std::min_element(points.begin(), points.end(), [&](Point3D &p1, Point3D &p2) {
            return p1.z < p2.z;
        });
        auto maxPoint = std::max_element(points.begin(), points.end(), [&](Point3D &p1, Point3D &p2) {
            return p1.z < p2.z;
        });
        float avg = (maxPoint->z + minPoint->z) / 2.0f;

        // Draw chart using calculated data
        glPointSize(3.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < xn * yn; i++) {
            float b = 1.0f - points[i].z / avg;
            float r = points[i].z / avg - 1.0f;
            if (b < 0.0f) b = 0.0f;
            if (r < 0.0f) r = 0.0f;
            float g = 1.0f - b - r;
            glColor4f(r, g, b, 0.5f);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }
        glEnd();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}