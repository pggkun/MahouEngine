#ifndef _MESH3D_H_
#define _MESH3D_H_

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "../camera.h"
#include "../transform.h"

class Mesh3D
{
    public:
        Mesh3D(const char *file_name,  Camera *cam, Shader *sh, Texture *t);
        ~Mesh3D();

        std::vector<float> vertices;
        GLuint vbo, vao, tex;
        GLint mdlvMtx, projMtx;
        glm::mat4 modelViewMatrix;
        Shader *shader;
        Texture *texture;
        glm::vec4 albedo;
        Camera *camera;
        Transform transform;

        void Update();
        void Draw();
};

#endif