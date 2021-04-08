#include "mesh.h"

mesh::mesh(const std::vector<float>& vertices, GLenum usage) : vertices_count(vertices.size()) {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], usage);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void mesh::load() {
    glBindVertexArray(this->vao);
}

void mesh::render() {
    glDrawArrays(GL_TRIANGLES, 0, this->vertices_count);
}

mesh::~mesh() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}