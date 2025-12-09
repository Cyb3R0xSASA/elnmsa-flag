#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> // GLM
#include <glm/vec3.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Read shader file into string
std::string loadShader(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Austrian Flag (GLM version)", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Load shaders
    std::string vertexCode = loadShader("shaders/vertexShader.glsl");
    std::string fragmentCode = loadShader("shaders/fragmentShader.glsl");

    const char *vertexSrc = vertexCode.c_str();
    const char *fragmentSrc = fragmentCode.c_str();

    // Compile vertex shader
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexSrc, NULL);
    glCompileShader(vShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Error:\n"
                  << infoLog << std::endl;
    }

    // Compile fragment shader
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragmentSrc, NULL);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader Error:\n"
                  << infoLog << std::endl;
    }

    // Link program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Linking Error:\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    // Austrian flag colors
    glm::vec3 RED(0.749f, 0.184f, 0.184f);
    glm::vec3 WHITE(1.0f, 1.0f, 1.0f);

    // Flag vertices with glm colors
    float vertices[] = {
        // x, y,     r, g, b
        -1.0f, 1.0f, RED.r, RED.g, RED.b,
        1.0f, 1.0f, RED.r, RED.g, RED.b,
        -1.0f, 0.333f, RED.r, RED.g, RED.b,
        1.0f, 0.333f, RED.r, RED.g, RED.b,

        -1.0f, 0.333f, WHITE.r, WHITE.g, WHITE.b,
        1.0f, 0.333f, WHITE.r, WHITE.g, WHITE.b,
        -1.0f, -0.333f, WHITE.r, WHITE.g, WHITE.b,
        1.0f, -0.333f, WHITE.r, WHITE.g, WHITE.b,

        -1.0f, -0.333f, RED.r, RED.g, RED.b,
        1.0f, -0.333f, RED.r, RED.g, RED.b,
        -1.0f, -1.0f, RED.r, RED.g, RED.b,
        1.0f, -1.0f, RED.r, RED.g, RED.b};

    unsigned int indices[] = {
        0, 1, 2, 1, 2, 3,
        4, 5, 6, 5, 6, 7,
        8, 9, 10, 9, 10, 11};

    // VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}