#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cstddef>

#include "render_window.h"
#include "shader.h"
#include "mesh.h"

struct voxel {
    glm::mat4 transform;
    bool active;
};

void handle_input(render_window& window, glm::vec3 &camera_position, glm::vec3& camera_up, glm::vec3& camera_forward, const double last_update,
 float& mouse_x, float& mouse_y, float& pitch, float& yaw, bool& should_update);
std::vector<std::vector<std::vector<voxel>>> create_grid(const size_t width, const size_t height, const size_t length, const float size);
std::vector<std::vector<std::vector<voxel>>> apply_rule(const std::vector<std::vector<std::vector<voxel>>>& grid,
 const size_t survive_min, const size_t survive_max, const size_t birth_min, const size_t birth_max);
size_t get_living_neighbours(const std::vector<std::vector<std::vector<voxel>>>& grid, const size_t x_pos, const size_t y_pos, const size_t z_pos);


//TODO: remove invisible faces
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

    render_window window(800, 600, "Game of Life 3D", true);

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
  
    glUniformMatrix4fv(projection_pos, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);

    const size_t width = 50, height = 50, length = 50;
    std::vector<std::vector<std::vector<voxel>>> grid = create_grid(width, height, length, 0.1f);

    double last_update = 0;
    float mouse_x = 400.0f, mouse_y = 300.0f, pitch = 0.0f, yaw = 0.0f;
    bool should_update = false;

    double last_grid_update = 0;

    while(!window.should_close()) {
        glClear(GL_DEPTH_BUFFER_BIT);
        window.clear(0.5f, 0.2f, 0.35f, 1.0f);

        handle_input(window, camera_position, camera_up, camera_forward, last_update, mouse_x, mouse_y, pitch, yaw, should_update);
        last_update = glfwGetTime();

        glm::mat4 view = glm::lookAt(camera_position, camera_forward + camera_position, camera_up);
        glUniformMatrix4fv(view_pos, 1, GL_FALSE, glm::value_ptr(view));

        if(should_update && (last_update - last_grid_update) >= 0.5f) {
            last_grid_update = last_update;
            grid = apply_rule(grid, 12, 26, 13, 14);
        } 

        for(int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                for(int z = 0; z < length; z++) {
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

void handle_input(render_window& window, glm::vec3 &camera_position, glm::vec3& camera_up, glm::vec3& camera_forward, const double last_update,
 float& mouse_x, float& mouse_y, float& pitch, float& yaw, bool& should_update) {
    float new_mouse_x, new_mouse_y;
    window.get_mouse_position(new_mouse_x, new_mouse_y);

    pitch -= 0.075f * (new_mouse_y - mouse_y);
    yaw += 0.075f * (new_mouse_x - mouse_x);
    mouse_x = new_mouse_x;
    mouse_y = new_mouse_y;

    if(pitch > 90.0f) {
        pitch = 90.0f;
    } else if(pitch < -90.0f) {
        pitch = -90.0f;
    }

    camera_forward = glm::vec3(cos(glm::radians(yaw)), sin(glm::radians(pitch)), sin(glm::radians(yaw)));
    
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
    if(window.pressed(GLFW_KEY_LEFT_SHIFT)) {
        camera_position -= camera_up * 5.0f * (float)delta_time;
    }
    if(window.pressed(GLFW_KEY_SPACE)) {
        camera_position += camera_up * 5.0f * (float)delta_time;
    }
    
    if(window.pressed(GLFW_KEY_ESCAPE)) {
        window.close();
    }

    if(window.pressed(GLFW_KEY_E)) {
        should_update = true;
    }
    if(window.pressed(GLFW_KEY_R)) {
        should_update = false;
    }
}

std::vector<std::vector<std::vector<voxel>>> create_grid(const size_t width, const size_t height, const size_t length, const float size) {
    std::vector<std::vector<std::vector<voxel>>> grid;
    grid.reserve(width);
    for(int x = 0; x < width; x++) {
        std::vector<std::vector<voxel>> x_row;
        x_row.reserve(height);
        for(int y = 0; y < height; y++) {
            std::vector<voxel> y_row;
            y_row.reserve(length);
            for(int z = 0; z < length; z++) {
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::scale(transform, glm::vec3(size, size, size));
                transform = glm::translate(transform, glm::vec3(x, y, z));
                y_row.push_back(voxel{transform, (bool)(rand() % 2)});
            }
            x_row.push_back(y_row);
        }
        grid.push_back(x_row);
    }

    return grid;
}

std::vector<std::vector<std::vector<voxel>>> apply_rule(const std::vector<std::vector<std::vector<voxel>>>& grid,
 const size_t survive_min, const size_t survive_max, const size_t birth_min, const size_t birth_max) {
    std::vector<std::vector<std::vector<voxel>>> new_grid = grid; 
    for(int x = 0; x < grid.size(); x++) {
        for(int y = 0; y < grid[x].size(); y++) {
            for(int z = 0; z < grid[x][y].size(); z++) {
                size_t living_neighbours = get_living_neighbours(grid, x, y, z);

                if(grid[x][y][z].active) {
                    if(living_neighbours < survive_min || living_neighbours > survive_max) {
                        new_grid[x][y][z].active = false;
                    }
                } else {
                    if(living_neighbours >= birth_min && living_neighbours <= birth_max) {
                        new_grid[x][y][z].active = true;
                    }
                }
            }
        }
    }

    return new_grid;
}

size_t get_living_neighbours(const std::vector<std::vector<std::vector<voxel>>>& grid, const size_t x_pos, const size_t y_pos, const size_t z_pos) {
    size_t living_neighbours = 0;

    for(int x = std::max(0, (int)x_pos - 1); x < std::min((int)grid.size(), (int)x_pos + 2); x++) {
        for(int y = std::max(0, (int)y_pos - 1); y < std::min((int)grid[x].size(), (int)y_pos + 2); y++) {
            for(int z = std::max(0, (int)z_pos - 1); z < std::min((int)grid[x][y].size(), (int)z_pos + 2); z++) {
                if(x != x_pos || y != y_pos || z != z_pos) {
                    living_neighbours += grid[x][y][z].active;
                }
            }
        }
    }

    return living_neighbours;
}