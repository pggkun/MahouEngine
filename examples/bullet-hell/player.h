#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <game_object.h>
#include <algorithm>
#include "projectile_scene.h"
#include <sprite_single_vertex_shader.h>
#include <sprite_single_frag_shader.h>

class Player : public GameObject
{
protected:
    static int global_count;
    int self_count;
    glm::mat4 modelViewMatrix;
    float bullet_layer;
    GameObject *life_vfx;
    Texture *texture;
    Texture *life_texture;

    float invencible_time; // 0.05s
    glm::vec4 current_color{1,1,1,1};

public:
    Player() : GameObject() {}
    ~Player();

    ProjectileScene *currScene;
    Shader *shader;
    void Start() override;
    void Update() override;
    float x_input;
    float y_input;
    float last_x;
    float last_y;
    bool focus;
    float hitbox_radio;

    bool CollisionCircleCircle(glm::vec3 collider, float col_rad);

    int hp;

    void Move(float x, float y, float z, float speed);
    void FadeIn();
    void FadeOut();
    void DrawLife();
};
#endif