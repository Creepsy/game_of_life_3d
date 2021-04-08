#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>

#include "render_window.h"
#include "mesh.h"

int main() {   
    std::vector<float> triangle_vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    render_window window{800, 600, "Game of Life 3D"};

    mesh triangle{triangle_vertices, GL_STATIC_DRAW};

    while(!window.should_close()) {
        window.clear(0.5f, 0.2f, 0.35f, 1.0f);

        //rendering goes here

        window.update();
    }

    return 0;
}