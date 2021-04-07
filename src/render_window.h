#pragma once

#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include <string>

class render_window {
    private:
        GLFWwindow* window;
    public:
        render_window(const unsigned int width, const unsigned int height, const std::string& title);
        bool should_close();
        void update();
        void clear(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha);
        ~render_window();
};
