#ifndef _ENEMY_H_
#define _ENEMY_H_
#include <game_entity.h>
#include "projectile_scene.h"
#include "projectile_object.h"
#include <material.h>

class Enemy : public GameEntity
{
protected:
    static int global_count;
    int self_count;

    float timer;
    float delay_timer;
    int delay_counter;
    
    glm::mat4 modelViewMatrix;
    float bullet_layer;

    glm::vec2 interval;
    std::list<glm::vec3> directions;
    
    void DelayedInstantiation(float delay);

public:
    Enemy(AnimatedSprite *spr, Camera *cam) : GameEntity(spr, cam) {}
    ~Enemy();
    void GetDirections(float amount);
    void Start() override;
    void Update() override;
    AnimatedSprite *bullet_sprite;
    float hitbox_radio;
    float dir_acc = 0.0f;

    Material *bullet_material;
    ProjectileScene *currScene;
    float shoot_interval = 1.0f;
    float bullet_speed = 1.5f;
    void Shoot();
    void GenerateBullet(glm::vec3 dir, glm::vec3 acc, float speed, glm::vec3 pos);
    void ShootMultiDirection(float speed, glm::vec3 acc);
    void ShootMultiDirDelayed(float speed, float delay, glm::vec3 acc);
    void SimpleMoveUp();
    void SimpleMoveDown();
};
#endif