#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
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

struct Vertex2DColor {
    glm::vec2 m_position;
    glm::vec3 m_color;

    Vertex2DColor(){};
    Vertex2DColor(glm::vec2 position, glm::vec3 color)
        : m_position(position), m_color(color){};
};

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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP1", nullptr, nullptr);
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

    // C:/Users/Quentin/Desktop/imac/s4/tpSynthese/TP1/shaders
    /// home/6ima2/quentin.augey/Documents/s4/synthese_image/GLImac-Template/TP1/shaders
    glimac::FilePath applicationPath("C:/Users/Quentin/Desktop/imac/s4/tpSynthese/TP1/shaders");

    // std::cout << argv[0] << "\n";

    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
                                                  applicationPath.dirPath() + "shaders/triangle.fs.glsl");
    program.use();

    GLuint vbo;
    glGenBuffers(1, &vbo);

    // GLuint vbo;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Vertex2DColor vertices[] = {
    //     Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
    //     Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),
    //     Vertex2DColor(glm::vec2(0, 0.5), glm::vec3(0, 0, 1)),
    // };

    std::vector<Vertex2DColor> points;
    points.push_back(Vertex2DColor(glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    const double nb_points = 27;
    const double rayon     = 0.4;

    for (double i = 0; i < nb_points + 1; i++) {
        points.push_back(Vertex2DColor(glm::vec2(rayon * glm::cos(2.f * glm::pi<double>() * (i / nb_points)), rayon * glm::sin(2.f * glm::pi<double>() * (i / nb_points))), glm::vec3(0, 0, 1)));
        // points.push_back(Vertex2DColor(glm::vec2(rayon * glm::cos(2.f * glm::pi<double>() * ((i + 1) / nb_points)), rayon * glm::sin(2.f * glm::pi<double>() * ((i + 1) / nb_points))), glm::vec3(0, 0, 1)));
    }
    // GLfloat vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};

    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vertex2DColor), points.data(), GL_STATIC_DRAW);

    glBindBuffer(0, vbo);

    // => Creation du IBO
    GLuint ibo;
    glGenBuffers(1, &ibo);

    // => On bind sur GL_ELEMENT_ARRAY_BUFFER, cible reservée pour les IBOs
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    uint32_t indices[84];
    int      k = 1;
    for (GLsizei i = 0; i < 83; i++) {
        if (i % 3 == 0) {
            indices[i] = 0;
            k--;
        }
        else {
            indices[i] = k;
            k++;
        }
    }
    indices[83] = indices[2];

    for (int i = 0; i < 84; i++) {
        std::cout << indices[i] << " ";
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 84 * sizeof(uint32_t), &indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    const GLuint VERTEX_ATTR_POSITION = 3;
    const GLuint VERTEX_ATTR_COLOR    = 8;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), 0);
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, m_color)));

    glBindBuffer(0, vbo);

    glBindVertexArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.f, 0.5f, 0.5f, 1.f);

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

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