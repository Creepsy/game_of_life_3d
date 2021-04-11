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

void handle_input(render_window& window, glm::vec3 &camera_position, glm::vec3& camera_up, glm::vec3& camera_forward, const double last_update);

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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::vec3 camera_position(1.0f, 1.0f, 5.0f);
    glm::vec3 camera_forward(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up(0.0f, 1.0f, 0.0f);
   /* glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(1.0f, 1.0f, 5.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(view_pos, 1, GL_FALSE, glm::value_ptr(view));*/
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

    double last_update = 0;

    while(!window.should_close()) {
        glClear(GL_DEPTH_BUFFER_BIT);
        window.clear(0.5f, 0.2f, 0.35f, 1.0f);

        handle_input(window, camera_position, camera_up, camera_forward, last_update);
        last_update = glfwGetTime();

        glm::mat4 view = glm::lookAt(camera_position, camera_forward + camera_position, camera_up);
        glUniformMatrix4fv(view_pos, 1, GL_FALSE, glm::value_ptr(view));

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

void handle_input(render_window& window, glm::vec3 &camera_position, glm::vec3& camera_up, glm::vec3& camera_forward, const double last_update) {
    double delta_time = glfwGetTime() - last_update;
    if(window.pressed(GLFW_KEY_W)) {
        camera_position += camera_forward * 5.0f * (float)delta_time;
    } 
    if(window.pressed(GLFW_KEY_S)) {
        camera_position -= camera_forward * 5.0f * (float)delta_time;
    }
    if(window.pressed(GLFW_KEY_A)) {
        camera_position -= glm::cross(camera_forward, camera_up) * 5.0f * (float)delta_time;
    }
    if(window.pressed(GLFW_KEY_D)) {
        camera_position += glm::cross(camera_forward, camera_up) * 5.0f * (float)delta_time;
    }
}