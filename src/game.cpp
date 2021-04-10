#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <random>

#include "render_window.h"
#include "shader.h"
#include "mesh.h"

struct voxel {
    glm::mat4 transform;
    bool active;
};

int main() { 
    srand(time(nullptr));

    std::vector<float> cube_vertices {
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        0.0, 0.0, 0.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        0.0, 1.0, 1.0
    };

    render_window window(800, 600, "Game of Life 3D");

    shader_program basic_shader("shaders/basic_shader.vs", "shaders/basic_shader.fs");
    mesh cube(cube_vertices, GL_STATIC_DRAW);
    
    int transform_pos = basic_shader.get_uniform_location("transform");
    int projection_pos = basic_shader.get_uniform_location("projection");
    int view_pos = basic_shader.get_uniform_location("view");

    basic_shader.enable();
    cube.load();

    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(800.0 / 600.0), 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-1.0f, -1.0f, -5.0f)); 

    glUniformMatrix4fv(view_pos, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_pos, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);

    std::vector<std::vector<std::vector<voxel>>> grid;
    grid.reserve(10);
    for(int x = 0; x < 10; x++) {
        std::vector<std::vector<voxel>> x_row;
        x_row.reserve(10);
        for(int y = 0; y < 10; y++) {
            std::vector<voxel> y_row;
            y_row.reserve(10);
            for(int z = 0; z < 10; z++) {
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::scale(transform, glm::vec3(0.2, 0.2, 0.2));
                transform = glm::translate(transform, glm::vec3(x, y, z));
                y_row.push_back(voxel{transform, (bool)(rand() % 2)});
            }
            x_row.push_back(y_row);
        }
        grid.push_back(x_row);
    }


    while(!window.should_close()) {
        glClear(GL_DEPTH_BUFFER_BIT);
        window.clear(0.5f, 0.2f, 0.35f, 1.0f);

        for(int x = 0; x < 10; x++) {
            for(int y = 0; y < 10; y++) {
                for(int z = 0; z < 10; z++) {
                    if(grid[x][y][z].active) {
                        glUniformMatrix4fv(transform_pos, 1, GL_FALSE, glm::value_ptr(grid[x][y][z].transform));
                        cube.render();
                    }
                }
            }
        }
        
        window.update();
    }

    return 0;
}