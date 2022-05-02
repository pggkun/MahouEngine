#ifndef _ANIME_H_
#define _ANIME_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>
#include <cassert>
#include <vector>
#include "game_time.h"
#include "render/mesh.h"

const char *const vertexShaderSource = R"text(
    #version 330 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 vtxTexCoord;

    uniform mat4 mdlvMtx;
    uniform mat4 projMtx;
    

    void main()
    {
        // Calculate position
        vec4 pos = mdlvMtx * vec4(aPos, 1.0);
        gl_Position = projMtx * pos;
        vtxTexCoord = aTexCoord;
    }
)text";

const char *const fragmentShaderSource = R"text(
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

class [[deprecated("Use Sprite instead.")]] AnimatedSprite
{
    private:
        int current_animation_index;
        int current_game_frame;
        float StartX(int sheet_position);
        float StartY(int sheet_position);
        float EndX(int sheet_position);
        float EndY(int sheet_position);
        struct Vertex data[6];

    public:
        AnimatedSprite(int frmamt_x, int frmamt_y, float fldur, const void *file, int len);
        ~AnimatedSprite();
        GLuint s_program;
        GLuint s_vao, s_vbo, s_tex;
        GLint loc_mdlvMtx, loc_projMtx, loc_tex_diffuse, loc_color_diffuse;
        float frame_duration;
        int frame_amount_x;
        int frame_amount_y;
        float animation_time;
        GLuint CreateAndCompileShader(GLenum type, const char *source);
        std::vector<std::vector<int>> animations;

        glm::vec4 sprite_color;
        void ApplyTexture(const void *file, int len);
        void UpdateFrameData(float x_start, float x_end, float y_start, float y_end);
        void UpdateAnimation();
        void Draw();
        void PushAnimation(std::vector<int> frames);
        void SwitchAnimation(int animation_index, float duration);
};

#endif