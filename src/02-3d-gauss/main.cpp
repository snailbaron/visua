#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <algorithm>

struct Point3D
{
    GLfloat x, y, z;
    Point3D() : x(0.0f), y(0.0f), z(0.0f) {}
    Point3D(GLfloat ax, GLfloat ay, GLfloat az) : x(ax), y(ay), z(az) {}
};

/*
 * Calculate 2D gaussian.
 
 The formula is:

                  /           2            2  \
                  |   (x - x0)     (y - y0)   |
                - | ---------- + ----------  |
                  |           2            2  |
                  \  2 sigma_x    2 sigma_y   /
 f(x, y) = A * e

                    1
 where A = --------------------
           2 Pi sigma_x sigma_y

 Here we are using a simplified version:
 * The same sigma is used for X and Y dimensions
 * The distribution is 0-centered, eliminating x0 and y0.
*/
float Gaussian(float x, float y, float sigma)
{
    return exp(-0.5f * (x * x) / (sigma * sigma) -0.5f * (y * y) / (sigma * sigma)) / (sigma * sigma * 2.0f * (float)M_PI);
}

void Draw2DHeatMap(const std::vector<Point3D> &points)
{
    auto minPoint = std::min_element(points.begin(), points.end(), [&](const Point3D &p1, const Point3D &p2) { return p1.z < p2.z; });
    auto maxPoint = std::max_element(points.begin(), points.end(), [&](const Point3D &p1, const Point3D &p2) {
        return p1.z < p2.z;
    }); 
    const float middle = (maxPoint->z + minPoint->z) / 2;

    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (auto i = points.begin(); i != points.end(); i++) {
        float value = i->z;
        float b = 1.0f - value/middle;
        float r = value/middle - 1.0f;
        if (b < 0) b = 0;
        if (r < 0) r = 0;
        float g = 1.0f - b - r;
        glColor4f(r, g, b, 0.5f);
        glVertex3f(i->x, i->y, 0.0f);
    }
    glEnd();
}

void GaussianDemo(float sigma)
{
    const int xn = 200;
    const int yn = 200;
    std::vector<Point3D> points(xn * yn);
    for (int i = 0; i < xn; i++) {
        for (int j = 0; j < yn; j++) {
            float x = i * 2.0f / (xn - 1) - 1.0f;
            float y = j * 2.0f / (yn - 1) - 1.0f;
            float z = Gaussian(x, y, sigma);
            points[i * yn + j] = Point3D(x, y, z);
        }
    }
    Draw2DHeatMap(points);
}

int main()
{
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow *window = glfwCreateWindow(1024, 768, "3D Gaussian", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float xscale, yscale;
    if (width >= height) {
        xscale = (float)width / height;
        yscale = 1.0f;
    } else {
        xscale = 1.0f;
        yscale = (float)height / width;
    }
        
    float aspectRatio = (float)width / height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-xscale, xscale, -yscale, yscale, 1.0f, -1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glEnable(GL_POINT_SMOOTH);

    float sigmaMin = 0.01f;
    float sigmaMax = 0.5f;
    float sigmaDelta = 0.01f;
    int direction = 1;
    float sigma = sigmaMin;
    while (!glfwWindowShouldClose(window)) {
        if (sigma <= sigmaMin) {
            sigma = sigmaMin;
            direction = 1;
        } else if (sigma >= sigmaMax) {
            sigma = sigmaMax;
            direction = -1;
        }
        sigma += direction * sigmaDelta;
        GaussianDemo(sigma);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}