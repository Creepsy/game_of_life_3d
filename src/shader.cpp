#include "shader.h"

#include <fstream>
#include <stdexcept>
#include <cstddef>
#include <iostream>

shader_program::shader_program(const std::string& vertex_shader_path, const std::string& fragment_shader_path) {
    unsigned int vertex_shader = this->load_shader(vertex_shader_path, GL_VERTEX_SHADER);
    unsigned int fragment_shader;
    try {
        fragment_shader = this->load_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
    } catch(...) {
        glDeleteShader(vertex_shader);
        return;
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader);
    glAttachShader(this->id, fragment_shader);
    glLinkProgram(this->id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success) {
        char log[512];
        glGetShaderInfoLog(this->id, 512, nullptr, log);
        std::cerr << log << std::endl;
        throw std::runtime_error("Shaderprogram linking failed!");
    }
}

unsigned int shader_program::load_shader(const std::string& path, GLenum type) {
    std::ifstream shader_file;
    shader_file.open(path, std::ios::ate);
    if(!shader_file.is_open()) throw std::runtime_error("Unable to load the shader!");

    size_t shader_size = shader_file.tellg();
    shader_file.seekg(std::ios::beg);

    char* shader_data = new char[shader_size];
    shader_file.read(shader_data, shader_size);
    shader_file.close();

    unsigned int shader;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_data, nullptr);
    delete[] shader_data;

    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {

        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << log << std::endl;
        throw std::runtime_error("Shader compilation failed!");
    }

    return shader;
}

void shader_program::enable() {
    glUseProgram(this->id);
}

shader_program::~shader_program() {
    glDeleteProgram(this->id);
}
