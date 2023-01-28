#include "mesh_3d.h"
#include "../obj_to_buffer.h"

Mesh3D::Mesh3D(const char *file_name, Camera *cam, Shader *sh, Texture *t)
{
    vertices = get_buffer_from_wavefront(file_name);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    shader = sh;
    texture = t;
    camera = cam;

    transform = Transform();
    transform.position = glm::vec3{0.0f, 0.0f, 0.0f};
    transform.rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    transform.angle = 0.0f;
    transform.scaleAmount = glm::vec3{1.0f, 1.0f, 1.0f};

    glGenTextures(1, &this->tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->image);
}

Mesh3D::~Mesh3D()
{
}

void Mesh3D::Update()
{

    shader->use();
    modelViewMatrix = camera->viewMatrix() * transform.modelMatrix();
    shader->setMat4("mdlvMtx", modelViewMatrix);
    // shader->setInt("tex_diffuse", 0);
}

void Mesh3D::Draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &this->vertices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 5);
    // GameTime::draw_calls += 1;
    glBindVertexArray(0);
}
