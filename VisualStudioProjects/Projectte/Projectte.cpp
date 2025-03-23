#include <glew.h>
#include <glfw3.h>
#include <iostream>
#define M_PI 3.14159265358979323846
#include <cmath>

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

GLuint VBO, VAO, EBO;
GLuint shader_program;

const char* vertex_shader = R"(
#version 410 core
layout (location = 0) in vec3 vp;
void main() {
    gl_Position = vec4(vp, 1.0);
})";

const char* fragment_shader = R"(
#version 410 core
out vec4 frag_colour;
uniform vec4 ourColor;
void main() {
    frag_colour = ourColor;
})";

void compile_shader(GLuint& shader, const char* shader_code, GLenum shader_type) {
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void create_shader_program() {
    GLuint vs, fs;
    compile_shader(vs, vertex_shader, GL_VERTEX_SHADER);
    compile_shader(fs, fragment_shader, GL_FRAGMENT_SHADER);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void init_geometry() {
    const int num_segments = 360; 
    const float a = 0.5f; 
    const float b = 0.3f; 

    float vertices[3 * (num_segments + 1)]; 
    GLuint indices[3 * num_segments]; 
    
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;

    
    for (int i = 0; i < num_segments; ++i) {
        float angle = (2.0f * M_PI * i) / num_segments;
        vertices[3 * (i + 1)] = a * cos(angle);  
        vertices[3 * (i + 1) + 1] = b * sin(angle); 
        vertices[3 * (i + 1) + 2] = 0.0f; 
    }

    
    for (int i = 0; i < num_segments; ++i) {
        indices[3 * i] = 0; 
        indices[3 * i + 1] = i + 1;
        indices[3 * i + 2] = (i + 1) % num_segments + 1; 
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawEllipse(float time) {
    float r = (sin(time) + 1.0f) / 2.0f; 
    float g = (cos(time) + 1.0f) / 2.0f;

    glUseProgram(shader_program);

    
    GLint colorLoc = glGetUniformLocation(shader_program, "ourColor");
    glUniform4f(colorLoc, r, g, 0.5f, 1.0f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * 360, GL_UNSIGNED_INT, 0); 
    glBindVertexArray(0);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    create_shader_program();
    init_geometry();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        drawEllipse(timeValue);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
