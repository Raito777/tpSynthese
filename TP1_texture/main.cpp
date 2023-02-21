#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/Program.hpp>
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

struct Vertex2DUV {
    glm::vec2 m_position;
    glm::vec2 m_texturePosition;

    Vertex2DUV(){};
    Vertex2DUV(glm::vec2 position, glm::vec2 texturePosition)
        : m_position(position), m_texturePosition(texturePosition){};
};

glm::mat3 translate(float tx, float ty)
{
    return glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
}

glm::mat3 scale(float sx, float sy)
{
    return glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
}

glm::mat3 rotate(float a)
{
    float b = glm::radians(a);
    return glm::mat3(glm::vec3(glm::cos(b), sin(b), 0), glm::vec3(-glm::sin(b), glm::cos(b), 0), glm::vec3(0, 0, 1));
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP2", nullptr, nullptr);

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

    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);


///home/6ima2/quentin.augey/Documents/s4/synthese_image/GLImac-Template/TP1/shaders
//home/6ima2/quentin.augey/Documents/s4/synthese_image/tpsynthese/assets/textures/triforce_2.png
//C:/Users/Quentin/Desktop/imac/s4/tpSynthese/assets/textures/triforce_2.png
    std::unique_ptr<glimac::Image> pImage = glimac::loadImage("/home/6ima2/quentin.augey/Documents/s4/synthese_image/tpSynthese/assets/textures/triforce_2.png");

    std::cout << argv[0] << "\n";

    GLuint texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    // transparence
    //  glEnable(GL_BLEND);
    //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->getWidth(), pImage->getHeight(), 0, GL_RGBA, GL_FLOAT, pImage->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    ///home/6ima2/quentin.augey/Documents/s4/synthese_image/GLImac-Template/TP1/shaders
    //C:/Users/Quentin/Desktop/imac/s4/tpSynthese/TP1/shaders
    glimac::FilePath applicationPath("/home/6ima2/quentin.augey/Documents/s4/synthese_image/tpSynthese/TP1/shaders");

    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "shaders/tex2D.vs.glsl",
                                                  applicationPath.dirPath() + "shaders/tex2D.fs.glsl");
    program.use();

    GLint uTexture = glGetUniformLocation(program.getGLId(), "uTexture");

    GLuint vbo;
    glGenBuffers(1, &vbo);

    // GLuint vbo;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex2DUV vertices[] = {
        Vertex2DUV(glm::vec2(-0.2, -0.2), glm::vec2(0, 1)),
        Vertex2DUV(glm::vec2(0.2, -0.2), glm::vec2(1, 1)),
        Vertex2DUV(glm::vec2(0, 0.2), glm::vec2(0.5, 0))};

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);

    glBindBuffer(0, vbo);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_TEXTURE  = 1;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), 0);
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, m_texturePosition)));

    glBindBuffer(0, vbo);

    glBindVertexArray(0);

    // GLint uTime         = glGetUniformLocation(program.getGLId(), "uTime");
    // GLint uRotateMatrix = glGetUniformLocation(program.getGLId(), "uRotateMatrix");
    GLint uModelMatrix  = glGetUniformLocation(program.getGLId(), "uModelMatrix");

    float angle = 1.f;

    std::vector<glm::mat3> transformations;
    transformations.push_back(translate(-0.5f, -0.5f));
    transformations.push_back(translate(0.5f, 0.5f));
    transformations.push_back(translate(-0.5f, 0.5f));
    transformations.push_back(translate(0.5f, -0.5f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);

        glBindTexture(GL_TEXTURE_2D, texture);

        glUniform1i(uTexture, 0);

        transformations[0] = transformations[0] * rotate(angle);
        transformations[1] = transformations[1] * rotate(-angle);

        for (size_t i = 0; i < transformations.size(); i++) {
            glUniformMatrix3fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(transformations[i]));
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        glBindBuffer(0, vbo);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteTextures(0, &texture);
    glDeleteBuffers(0, &vbo);
    glDeleteVertexArrays(0, &vao);

    glfwTerminate();
    return 0;
}