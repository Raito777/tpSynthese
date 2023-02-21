#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/common.hpp>
#include <glimac/glm.hpp>
#include <iostream>
#include <vector>

int window_width  = 1280;
int window_height = 720;

static void key_callback(GLFWwindow* /*window*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{
}

static void mouse_button_callback(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/)
{
}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/)
{
}

static void cursor_position_callback(GLFWwindow* /*window*/, double /*xpos*/, double /*ypos*/)
{
}

static void size_callback(GLFWwindow* /*window*/, int width, int height)
{
    window_width  = width;
    window_height = height;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a window and its OpenGL context */
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP4", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    // C:/Users/Quentin/Desktop/imac/s4/tpSynthese/TP1/shaders
    ////home/6ima2/quentin.augey/Documents/s4/synthese_image/tpSynthese/bin/Debug/TP1

    glimac::FilePath applicationPath(argv[0]);

    // std::cout << "chemin : " << applicationPath.dirPath() << "\n";

    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "TP1/shaders/3D.vs.glsl",
                                                  applicationPath.dirPath() + "TP1/shaders/normals.fs.glsl");

    program.use();

    glimac::Sphere             sphere(1, 32, 16);
    const glimac::ShapeVertex* vertices = sphere.getDataPointer();

    std::cout << "normals : " << &vertices->position << "\n";

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glimac::ShapeVertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(0, vbo);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION  = 0;
    const GLuint VERTEX_ATTR_NORMAL    = 1;
    const GLuint VERTEX_ATTR_TexCoords = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TexCoords);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    glBindBuffer(0, vbo);

    glBindVertexArray(0);

    GLint uMVPMatrix    = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjMatrix, MVMatrix, NormalMatrix;

    ProjMatrix   = glm::perspective(glm::radians(70.f), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 100.f);
    MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));
    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.f, 0.75f, 0.75f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        glBindVertexArray(0);
        glBindBuffer(0, vbo);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteBuffers(0, &vbo);
    glDeleteVertexArrays(0, &vao);

    glfwTerminate();
    return 0;
}