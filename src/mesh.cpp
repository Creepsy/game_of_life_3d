#include "mesh.h"

mesh::mesh(const std::vector<float>& vertices, GLenum usage) {
    glGenBuffers(1, &this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], usage);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

mesh::~mesh() {
    glDeleteBuffers(1, &this->vbo);
}