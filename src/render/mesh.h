#ifndef _MESH_H_
#define _MESH_H_

#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include "shader.h"

struct Vertex
{
    float position[3];
    float texcoord[2];
};

class Mesh
{
    public:
        Mesh();
        ~Mesh();

        std::vector<Vertex> vertices;
        GLuint vbo;
        GLuint vao;
        GLint mdlvMtx, projMtx;
        glm::mat4 modelViewMatrix;

        void SetupPlane(float x_start, float x_end, float y_start, float y_end, float x, float y, float w, float h);
        void Initialize();
        void Update();
};

#endif