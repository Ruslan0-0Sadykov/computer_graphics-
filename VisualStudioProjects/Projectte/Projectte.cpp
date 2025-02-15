#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int NUM_SEGMENTS = 100; // Количество сегментов эллипса

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

void drawEllipse(float cx, float cy, float rx, float ry) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.5f, 0.2f);

    glVertex2f(cx, cy); // Центральная точка

    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float angle = 2.0f * M_PI * i / NUM_SEGMENTS;
        float x = cx + rx * cos(angle);
        float y = cy + ry * sin(angle);
        glVertex2f(x, y);
    }

    glEnd();
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Ellipse Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawEllipse(0.0f, 0.0f, 0.5f, 0.3f); // Эллипс в центре экрана

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}