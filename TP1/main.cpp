#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <glimac/Image.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/TrackballCamera.hpp>
#include <glimac/common.hpp>
#include <glimac/getTime.hpp>
#include <glimac/glm.hpp>
#include <iostream>
#include <vector>

int window_width  = 1280;
int window_height = 720;

const int Z = 87;
const int Q = 65;
const int S = 83;
const int D = 68;

struct Inputs {
    double    yoffset;
    int       mouseButton;
    int       key;
    glm::vec2 currentPosition;
    glm::vec2 previousPosition;
};

Inputs myInputs;

static void key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int /*action*/, int /*mods*/)
{
    myInputs.key = key;
    std::cout << key << "\n";
}

static void mouse_button_callback(GLFWwindow* /*window*/, int button, int /*action*/, int /*mods*/)
{
    if (myInputs.mouseButton != 0 && button == 0) {
        myInputs.mouseButton = button;
    }
    else {
        myInputs.mouseButton = -1;
    }
    std::cout << myInputs.mouseButton << "\n";
}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
    myInputs.yoffset = -yoffset * 0.5;
}

static void cursor_position_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
    myInputs.currentPosition = glm::vec2(xpos, ypos);
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

    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "TP1/shaders/3D.vs.glsl",
                                                  applicationPath.dirPath() + "TP1/shaders/pointlight.fs.glsl");

    program.use();

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

    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjMatrix, MVMatrix, NormalMatrix;

    std::vector<glm::mat4> transformationsMv;

    GLint uMVPMatrix    = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    size_t nbSphere = 32;

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

    // getting cursor position

    std::vector<glm::vec3> randAxes;
    std::vector<glm::vec3> randPoses;

    for (size_t i = 0; i < nbSphere; i++) {
        randAxes.push_back(glm::vec3(glm::sphericalRand<float>(1)));
        randPoses.push_back(glm::vec3(glm::sphericalRand<float>(2)));
    }

    for (size_t i = 0; i < nbSphere; i++) {
        transformationsMv.push_back(MVMatrix);
    }

    glimac::FreeflyCamera camera;

    GLint uAmbient        = glGetUniformLocation(program.getGLId(), "uAmbient");
    GLint uKd             = glGetUniformLocation(program.getGLId(), "uKd");
    GLint uKs             = glGetUniformLocation(program.getGLId(), "uKs");
    GLint uShininess      = glGetUniformLocation(program.getGLId(), "uShininess");
    GLint uLightPos_vs    = glGetUniformLocation(program.getGLId(), "uLightPos_vs");
    GLint uLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");

    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;
    std::vector<glm::vec3> _uLightIntensity;

    for (size_t i = 0; i < nbSphere + 1; i++) {
        _uKd.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        _uKs.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        _uLightIntensity.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        _uShininess.push_back(glm::linearRand(0.f, 1.0f));
    }

    glm::vec3 _uLightPos_vs = glm::vec3(0.5, 0.5, 0.5);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera.moveFront(myInputs.yoffset);
        if (myInputs.currentPosition.x < myInputs.previousPosition.x && myInputs.mouseButton == -1) {
            camera.rotateLeft(-0.7f);
        }
        if (myInputs.currentPosition.y < myInputs.previousPosition.y && myInputs.mouseButton == -1) {
            camera.rotateUp(0.7f);
        }
        if (myInputs.currentPosition.x > myInputs.previousPosition.x && myInputs.mouseButton == -1) {
            camera.rotateLeft(0.7f);
        }
        if (myInputs.currentPosition.y > myInputs.previousPosition.y && myInputs.mouseButton == -1) {
            camera.rotateUp(-0.7f);
        }
        if (myInputs.key == Z) {
            camera.moveFront(0.1f);
            myInputs.key = -1;
        }
        if (myInputs.key == Q) {
            camera.moveLeft(0.1f);
            myInputs.key = -1;
        }
        if (myInputs.key == S) {
            camera.moveFront(-0.1f);
            myInputs.key = -1;
        }
        if (myInputs.key == D) {
            camera.moveLeft(-0.1f);
            myInputs.key = -1;
        }

        myInputs.previousPosition = myInputs.currentPosition;

        myInputs.yoffset     = 0;
        transformationsMv[0] = camera.getViewMatrix();

        glBindVertexArray(vao);

        MVMatrix = glm::rotate(transformationsMv[0], 0.f, glm::vec3(0, 1, 0));

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        glBindTexture(GL_TEXTURE_2D, 0);
        // glm::vec3 rotatedLightDir = _uLightDir_vs * glm::mat3(glm::rotate(glm::mat4(1.0f), glimac::getTime(), glm::vec3(0.0f, 0.0f, 1.0f)));
        // glUniform3fv(uLightDir_vs, 1, glm::value_ptr(glm::mat3(transformationsMv[0]) * rotatedLightDir));

        // glUniform3fv(uLightDir_vs, 1, glm::value_ptr(glm::vec3(glm::vec4(1, 1, 1, 0) * glm::rotate(glm::mat4(1.0f), glimac::getTime(), glm::vec3(0, 1, 0)) * camera.getViewMatrix())));

        glUniform3fv(uLightPos_vs, 1, glm::value_ptr(glm::vec3(camera.getViewMatrix() * glm::vec4(0.5, 0.5, 0.5, 0))));

        glUniform3fv(uLightIntensity, 1, glm::value_ptr(glm::vec3(2.f, 2.f, 2.f)));
        glUniform3fv(uAmbient, 1, glm::value_ptr(glm::vec3(0.f, 0.f, 0.f)));

        for (size_t i = 1; i < nbSphere; i++) {
            glUniform3fv(uKd, 1, glm::value_ptr(_uKd[i]));
            glUniform3fv(uKs, 1, glm::value_ptr(_uKs[i]));
            glUniform1f(uShininess, _uShininess[i]);

            transformationsMv[i] = glm::rotate(transformationsMv[0], 0.f, randAxes[i]);
            transformationsMv[i] = glm::translate(transformationsMv[i], randPoses[i]);         // Translation * Rotation * Translation
            transformationsMv[i] = glm::scale(transformationsMv[i], glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * transformationsMv[i]));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(transformationsMv[i]));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
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