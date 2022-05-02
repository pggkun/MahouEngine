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

const char *const vs = R"text(
    #version 330 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    layout (location = 3) in mat4 aInstanceMatrix;

    out vec2 vtxTexCoord;

    uniform mat4 projMtx;
    

    void main()
    {
        // Calculate position
        vec4 pos = aInstanceMatrix * vec4(aPos, 1.0);
        gl_Position = projMtx * pos;
        vtxTexCoord = aTexCoord;
    }
    )text";

const char *const fs = R"text(
    #version 330 core

    in vec2 vtxTexCoord;

    out vec4 fragColor;
    uniform sampler2D tex_diffuse;
    uniform vec4 color;

    void main()
    {
        vec4 texDiffuseColor = texture(tex_diffuse, vtxTexCoord);
        //vec3 fragLightColor = 1.0 + 1.0*texDiffuseColor.rgb;
        //fragColor = vec4(min(fragLightColor, 1.0), texDiffuseColor.a);

        fragColor = texDiffuseColor * color;
    }
    )text";

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
        std::vector<glm::mat4> modelMatrices;

        std::list<std::pair<Transform *, Mesh *>> objectsToDraw;
        void Bind();
        void Draw();
        void SetupMatrices();
};

#endif