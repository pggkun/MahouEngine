#include "sprite.h"

Sprite::Sprite(Mesh *m, float c, float r, float w, float h)
{
    mesh = m;
    cols = c;
    rows = r;
    height = 1.0f;
    width = ((w / cols) / (h / rows));
    animations.clear();
    GetEquidistantFrames();
    AddAnimation({0});
    animation_time = 0;
    current_animation_index = 0;
    current_frame_index = 0;
}

Sprite::~Sprite()
{

}

void Sprite::GetEquidistantFrames()
{
    frames.clear();
    for (int i=0; i<rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            float sx = (1.0 / cols) * j;
            float ex = (1.0 / cols) * (j + 1);
            float sy = (1.0 / rows) * i;
            float ey = (1.0 / rows) * (i + 1);
            AddFrame(sx, ex, sy, ey);
        }
    }
}

void Sprite::AddFrame(float x_start, float x_end, float y_start, float y_end)
{
    frames.push_back({x_start, x_end, y_start, y_end});
}


void Sprite::AddAnimation(std::vector<int> frms)
{
    std::vector<std::vector<float>> animation_frames;
    for(int i=0; i < frms.size(); i++)
    {
        animation_frames.push_back(frames[frms[i]]);
    }
    animations.push_back(animation_frames);
}

void Sprite::SwitchAnimation(int index, float duration)
{
    frame_duration = duration;
    current_animation_index = index;
}

void Sprite::Update()
{
    if ((animation_time) > (this->frame_duration * animations[current_animation_index].size()))
    {
        animation_time = 0;
    }
    int my_frame = (int)floor((animation_time) / this->frame_duration);
    mesh->SetupPlane(animations[current_animation_index][my_frame][0],
                     animations[current_animation_index][my_frame][1],
                     animations[current_animation_index][my_frame][2],
                     animations[current_animation_index][my_frame][3],
                     0,
                     0,
                     width,
                     height);
    mesh->Initialize();
    animation_time += GameTime::delta_time;
}