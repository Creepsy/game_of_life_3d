#pragma once

#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include <istream>
#include <string>

class shader_program {
    private:
        unsigned int id;

        unsigned int load_shader(const std::string& path, GLenum type);
    public:
        shader_program(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
        void enable();
        int get_uniform_location(const std::string& name);
        ~shader_program();
};

