#include "material.h"
#include <stdio.h>
#include "../game_time.h"
#include <iostream>
#define GLCall(x)   \
    GLClearError(); \
    x;              \
    GLLogCall(#x, __FILE__, __LINE__);

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Errorr] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

Material::Material()
{
    albedo = {1, 1, 1, 1};
}

Material::~Material()
{
    objectsToDraw.clear();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &tex);
}

void Material::SetProperties(Shader *sh, Texture *t, Camera *cam, glm::vec4 al)
{
    shader = sh;
    texture = t;
    camera = cam;
    albedo = al;

    glGenTextures(1, &this->tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->image);
}

void Material::SetupMatrices()
{
    modelMatrices.clear();
    for (std::list<std::pair<Transform *, Mesh *>>::iterator it = objectsToDraw.begin(); it != objectsToDraw.end(); ++it)
    {
        modelMatrices.push_back({(*it).second->vertices[0], (*it).second->modelViewMatrix});
        modelMatrices.push_back({(*it).second->vertices[1], (*it).second->modelViewMatrix});
        modelMatrices.push_back({(*it).second->vertices[2], (*it).second->modelViewMatrix});
        modelMatrices.push_back({(*it).second->vertices[3], (*it).second->modelViewMatrix});
        modelMatrices.push_back({(*it).second->vertices[4], (*it).second->modelViewMatrix});
        modelMatrices.push_back({(*it).second->vertices[5], (*it).second->modelViewMatrix});
    }
}

void Material::Draw()
{
    if (modelMatrices.size() > 0)
    {
        shader->use();
    shader->setInt("tex_diffuse", 0);
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, tex));
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * ((sizeof(Vertex)) + sizeof(glm::mat4)), &modelMatrices.front(), GL_STATIC_DRAW);
    int objectCounter = 0;
    for (std::list<std::pair<Transform *, Mesh *>>::iterator it = objectsToDraw.begin(); it != objectsToDraw.end(); ++it)
    {
            GLCall(glBindVertexArray((*it).second->vao));

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(Vertex), (void *)offsetof(TexMatrix, vertexinfo.position));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(Vertex), (void *)offsetof(TexMatrix, vertexinfo.texcoord));

            GLCall(glEnableVertexAttribArray(3));
            GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(Vertex), (void *)offsetof(TexMatrix, modelMtx)));
            GLCall(glEnableVertexAttribArray(4));
            GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(Vertex), (void *)(sizeof(glm::vec4) + offsetof(TexMatrix, modelMtx))));
            GLCall(glEnableVertexAttribArray(5));
            GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(Vertex), (void *)(2 * sizeof(glm::vec4) + offsetof(TexMatrix, modelMtx))));
            GLCall(glEnableVertexAttribArray(6));
            GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(Vertex), (void *)(3 * sizeof(glm::vec4) + offsetof(TexMatrix, modelMtx))));

            GLCall(glDrawArraysInstanced(GL_TRIANGLES, objectCounter * 6, 6, objectsToDraw.size()));
            objectCounter++;
    }
    GameTime::draw_calls += 1;
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}