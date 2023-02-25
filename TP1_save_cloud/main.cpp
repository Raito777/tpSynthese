#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/common.hpp>
#include <glimac/getTime.hpp>
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

    /// home/6ima2/quentin.augey/Documents/s4/synthese_image/GLImac-Template/TP1/shaders
    // home/6ima2/quentin.augey/Documents/s4/synthese_image/tpsynthese/assets/textures/triforce_2.png
    // C:/Users/Quentin/Desktop/imac/s4/tpSynthese/assets/textures/triforce_2.png
    std::unique_ptr<glimac::Image> earthImage = glimac::loadImage(applicationPath.dirPath() + "assets/textures/EarthMap.jpg");
    std::unique_ptr<glimac::Image> moonImage  = glimac::loadImage(applicationPath.dirPath() + "assets/textures/MoonMap.jpg");
    std::unique_ptr<glimac::Image> cloudImage = glimac::loadImage(applicationPath.dirPath() + "assets/textures/CloudMap.jpg");

    GLuint textureEarth;
    GLuint textureMoon;
    GLuint textureCloud;

    glGenTextures(1, &textureEarth);
    glBindTexture(GL_TEXTURE_2D, textureEarth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earthImage->getWidth(), earthImage->getHeight(), 0, GL_RGBA, GL_FLOAT, earthImage->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &textureMoon);
    glBindTexture(GL_TEXTURE_2D, textureMoon);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moonImage->getWidth(), moonImage->getHeight(), 0, GL_RGBA, GL_FLOAT, moonImage->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &textureCloud);
    glBindTexture(GL_TEXTURE_2D, textureCloud);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloudImage->getWidth(), cloudImage->getHeight(), 0, GL_RGBA, GL_FLOAT, cloudImage->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "TP1/shaders/3D.vs.glsl",
                                                  applicationPath.dirPath() + "TP1/shaders/multiTex3D.fs.glsl");

    program.use();

    GLint uTexture1 = glGetUniformLocation(program.getGLId(), "uTexture1");
    GLint uTexture2 = glGetUniformLocation(program.getGLId(), "uTexture2");

    glimac::Sphere             sphere(1, 32, 16);
    const glimac::ShapeVertex* vertices = sphere.getDataPointer();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(glimac::ShapeVertex), vertices, GL_STATIC_DRAW);

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

    std::vector<glm::mat4> transformationsMv;

    size_t nbSphere = 64;

    ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 100.f);
    MVMatrix   = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));

    transformationsMv.push_back(MVMatrix);

    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    std::vector<glm::vec3> randAxes;
    std::vector<glm::vec3> randPoses;

    for (size_t i = 0; i < nbSphere; i++) {
        randAxes.push_back(glm::vec3(glm::sphericalRand<float>(1)));
        randPoses.push_back(glm::vec3(glm::sphericalRand<float>(2)));
    }

    for (size_t i = 0; i < nbSphere; i++) {
        transformationsMv.push_back(MVMatrix);
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.f, 0.75f, 0.75f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        MVMatrix = glm::rotate(transformationsMv[0], glimac::getTime(), glm::vec3(0, 1, 0));

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureEarth); // la texture earthTexture est bindée sur l'unité GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureCloud);

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        glBindTexture(GL_TEXTURE_2D, textureMoon);
        glBindTexture(GL_TEXTURE_2D, textureCloud);

        glUniform1i(uTexture1, 0);
        glUniform1i(uTexture2, 1);

        glBindTexture(GL_TEXTURE_2D, 0);

        for (size_t i = 1; i < nbSphere; i++) {
            transformationsMv[i] = glm::rotate(transformationsMv[0], glimac::getTime(), randAxes[i]);
            transformationsMv[i] = glm::translate(transformationsMv[i], randPoses[i]);         // Translation * Rotation * Translation
            transformationsMv[i] = glm::scale(transformationsMv[i], glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * transformationsMv[i]));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(transformationsMv[i]));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureMoon); // la texture earthTexture est bindée sur l'unité GL_TEXTURE0
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

            glBindTexture(GL_TEXTURE_2D, textureMoon);
            glBindTexture(GL_TEXTURE_2D, textureCloud);

            glUniform1i(uTexture1, 0);
            // glUniform1i(uTexture2, 1);
        }

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        glBindBuffer(0, vbo);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    // glDeleteTextures(0, &textureEarth);
    // glDeleteTextures(0, &textureMoon);

    glDeleteBuffers(0, &vbo);
    glDeleteVertexArrays(0, &vao);

    glfwTerminate();
    return 0;
}