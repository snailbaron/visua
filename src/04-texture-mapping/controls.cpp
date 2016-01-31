#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "controls.hpp"

glm::mat4 g_viewMatrix;
glm::mat4 g_projMatrix;

glm::vec3 g_position = glm::vec3(0, 0, 5);  // camera position
const float speed = 3.0f;                   // 3 units per second
float g_initialFov = glm::pi<float>() * 0.4f;

void CalcMatrices(GLFWwindow *window)
{
    static double lastTime = glfwGetTime();
    double curTime = glfwGetTime();
    float deltaTime = static_cast<float>(curTime - lastTime);
    lastTime = curTime;

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glm::vec3 direction(0, 0, -1);
    glm::vec3 up = glm::vec3(0, -1, 0);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        g_position += direction * deltaTime * speed;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        g_position -= direction * deltaTime * speed;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        g_initialFov -= 0.1f * deltaTime * speed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        g_initialFov += 0.1f * deltaTime * speed;
    }

    g_projMatrix = glm::perspective(g_initialFov, (float)width / height, 0.1f, 100.0f);
    g_viewMatrix = glm::lookAt(g_position, g_position + direction, up);
}