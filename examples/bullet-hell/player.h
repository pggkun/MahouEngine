#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <game_entity.h>
#include "projectile_scene.h"

class Player : public GameEntity
{
protected:
    static int global_count;
    int self_count;
    glm::mat4 modelViewMatrix;
    float bullet_layer;
    GameEntity *life_vfx;

public:
    Player(AnimatedSprite *spr, Camera *cam) : GameEntity(spr, cam) {}
    ~Player();

    ProjectileScene *currScene;
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
    void Shoot();
    void ShootPattern1();
    void FadeIn();
    void FadeOut();
    void DrawLife();
};
#endif