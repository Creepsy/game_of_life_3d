#pragma once

#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include <vector>

class mesh {
    private:
        unsigned int vbo;
    public:
        mesh(const std::vector<float>& vertices, GLenum usage);
        ~mesh();
};


