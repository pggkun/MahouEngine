#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>

#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "../transform.h"
#include "../camera.h"
#include <list>
#include <vector>

#include <sprite_inst_vert_shader.h>
#include <sprite_inst_frag_shader.h>

struct TexMatrix
{
    Vertex vertexinfo;
    glm::mat4 modelMtx;
};

class Material
{
    private:
        GLuint vbo, vao, tex;

    public:
        Material();
        ~Material();

        void SetProperties(Shader *sh, Texture *t, Camera *cam, glm::vec4 al);

        Shader *shader;
        Texture *texture;
        glm::vec4 albedo;
        Camera *camera;
        int material_id;
        std::vector<TexMatrix> modelMatrices;

        std::list<std::pair<Transform *, Mesh *>> objectsToDraw;
        void Bind();
        void Draw();
        void SetupMatrices();
};

#endif