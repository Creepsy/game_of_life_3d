#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include <string>
#include <iostream>
#include <vector>

#include "render_window.h"
#include "shader.h"
#include "mesh.h"

int main() {  
    std::vector<float> triangle_vertices = {
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0.0, 0.5, 0.0
    };

    render_window window(800, 600, "Game of Life 3D");

    shader_program basic_shader("shaders/basic_shader.vs", "shaders/basic_shader.fs");
    mesh triangle(triangle_vertices, GL_STATIC_DRAW);
    
   // int time_pos = basic_shader.get_uniform_location("time");
    int transform_pos = basic_shader.get_uniform_location("transform");
    int projection_pos = basic_shader.get_uniform_location("projection");
    int view_pos = basic_shader.get_uniform_location("view");

    basic_shader.enable();
    triangle.load();

    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(800.0 / 600.0), 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

    glUniformMatrix4fv(view_pos, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_pos, 1, GL_FALSE, glm::value_ptr(projection));


    while(!window.should_close()) {
        window.clear(0.5f, 0.2f, 0.35f, 1.0f);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));  
        
        glUniformMatrix4fv(transform_pos, 1, GL_FALSE, glm::value_ptr(transform));

        //glUniform1f(time_pos, glfwGetTime());
        triangle.render();

        window.update();
    }

    return 0;
}