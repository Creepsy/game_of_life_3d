#include "render_window.h"

#include <stdexcept>

render_window::render_window(const unsigned int width, const unsigned int height, const std::string& title) {
    if(glfwInit() == GLFW_FALSE) throw std::runtime_error("Unable to initialize GLFW!");;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!this->window) {
        glfwTerminate();
        throw std::runtime_error("Unable to create a GLFW window!");
    }

    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Unable to initialize GLAD!");
    };

    glViewport(0, 0, width, height);
}

bool render_window::should_close() {
    return glfwWindowShouldClose(this->window);
}

void render_window::update() {
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

void render_window::clear(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha) {
    glClearColor(r, g, b, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

bool render_window::pressed(const int key) {
    return glfwGetKey(this->window, key) == GLFW_PRESS;
}

render_window::~render_window() {
    glfwTerminate();
}