#pragma once

#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include <vector>
#include <cstddef>

class mesh {
    private:
        unsigned int vbo;
        unsigned int vao;
        size_t vertices_count;
    public:
        mesh(const std::vector<float>& vertices, GLenum usage);
        void load();
        void render();
        ~mesh();
};


