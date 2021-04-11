#include "render_window.h"

#include <stdexcept>

render_window::render_window(const unsigned int width, const unsigned int height, const std::string& title, const bool disable_cursor) {
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

    if(disable_cursor) glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

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

void render_window::close() {
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void render_window::get_mouse_position(float& x, float& y) {
    double x_pos, y_pos;
    glfwGetCursorPos(this->window, &x_pos, &y_pos);
    x = (float) x_pos;
    y = (float) y_pos;
}


render_window::~render_window() {
    glfwTerminate();
}