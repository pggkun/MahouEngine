#include "mesh.h"
#include <stdio.h>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
}

void Mesh::SetupPlane(float x_start, float x_end, float y_start, float y_end, float x, float y, float w, float h)
{
    vertices = {
        {{x - w/2.0f, y - h/2.0f, 0.0f}, {x_start, y_end}},
        {{x + w/2.0f, y - h/2.0f, 0.0f}, {x_end, y_end}},
        {{x - w/2.0f, y + h/2.0f, 0.0f}, {x_start, y_start}},

        {{x + w/2.0f, y - h/2.0f, 0.0f}, {x_end, y_end}},
        {{x + w/2.0f, y + h/2.0f, 0.0f}, {x_end, y_start}},
        {{x - w/2.0f, y + h/2.0f, 0.0f}, {x_start, y_start}}
    };
}

void Mesh::Initialize()
{
    if (this->vao == 0) glGenVertexArrays(1, &this->vao);
    if (this->vbo == 0) glGenBuffers(1, &this->vbo);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
