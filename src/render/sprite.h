#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <gl/glcorearb.h>
#include <gl/glext.h>
#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include "mesh.h"
#include "material.h"
#include "../game_time.h"

class Sprite
{
    private:
        float animation_time;

    public:
        Sprite(Mesh *m, float c, float r, float w, float h);
        ~Sprite();

        float width, height;
        std::vector<std::vector<float>> frames;
        std::vector<std::vector<std::vector<float>>> animations;

        float frame_duration;
        int current_frame_index;
        int current_animation_index;
        int cols;
        int rows;

        Mesh *mesh;

        void GetEquidistantFrames();
        void AddFrame(float x_start, float x_end, float y_start, float y_end);
        void AddAnimation(std::vector<int> frms);
        void SwitchAnimation(int index, float duration);
        void Update();
};

#endif