#pragma once

#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include <string>

class render_window {
    private:
        GLFWwindow* window;
    public:
        render_window(const unsigned int width, const unsigned int height, const std::string& title, const bool disable_cursor = false);
        render_window(const render_window& other) = delete;
        render_window(render_window&& tmp) = delete;
        render_window& operator=(const render_window& other) = delete;
        render_window& operator=(render_window&& tmp) = delete;
        bool should_close();
        void update();
        void clear(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha);
        bool pressed(const int key);
        void close();
        void get_mouse_position(float& x, float& y);
        ~render_window();
};