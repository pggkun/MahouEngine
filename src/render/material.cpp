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
        //std::cout << "[OpenGL Errorr] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

Material::Material()
{
    //glGenBuffers(1, &vbo);
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

void Material::Bind()
{
    if (modelMatrices.size() > 0)
    {
        for (std::list<std::pair<Transform *, Mesh *>>::iterator it = objectsToDraw.begin(); it != objectsToDraw.end(); ++it)
        {
            //(*it).second->Initialize();
            GLCall(glBindVertexArray((*it).second->vao));
            // set attribute pointers for matrix (4 times vec4)
            GLCall(glEnableVertexAttribArray(3));
            GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)0));
            GLCall(glEnableVertexAttribArray(4));
            GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4))));
            GLCall(glEnableVertexAttribArray(5));
            GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(2 * sizeof(glm::vec4))));
            GLCall(glEnableVertexAttribArray(6));
            GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(3 * sizeof(glm::vec4))));

            GLCall(glVertexAttribDivisor(3, 1));
            GLCall(glVertexAttribDivisor(4, 1));
            GLCall(glVertexAttribDivisor(5, 1));
            GLCall(glVertexAttribDivisor(6, 1));

            GLCall(glBindVertexArray(0));
        }
    }
}

void Material::SetupMatrices()
{
    modelMatrices.clear();
    for (std::list<std::pair<Transform *, Mesh *>>::iterator it = objectsToDraw.begin(); it != objectsToDraw.end(); ++it)
    {
        modelMatrices.push_back((*it).second->modelViewMatrix);
    }
    if(modelMatrices.size() > 0)
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices.front(), GL_STATIC_DRAW));
    }
    //glDeleteBuffers(1, &vbo);

    
}

void Material::Draw()
{
    if (modelMatrices.size() > 0)
    {
        shader->use();
    shader->setInt("tex_diffuse", 0);
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, tex));
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    for (std::list<std::pair<Transform *, Mesh *>>::iterator it = objectsToDraw.begin(); it != objectsToDraw.end(); ++it)
    {
        // glBindBuffer(GL_ARRAY_BUFFER,(*it).second->vbo);
        GLCall(glBindVertexArray((*it).second->vao));
        // glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>((*it).second->vertices.size()), GL_UNSIGNED_INT, 0, objectsToDraw.size());
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, objectsToDraw.size()));
        
        GLCall(glBindVertexArray(0));
    }
    GameTime::draw_calls += 1;
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}