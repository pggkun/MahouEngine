#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "player.h"
#include <game_object.h>
#include "projectile_scene.h"
#include "projectile_object.h"
#include <material.h>

class Enemy : public GameObject
{
    protected:
        static int global_count;
        int self_count;

        float timer;
        float delay_timer;
        int delay_counter;
        float move_counter = 0;
        
        glm::mat4 modelViewMatrix;
        float bullet_layer;

        glm::vec2 interval;
        std::list<glm::vec3> directions;
        void GetDirections(float amount);
        void DelayedInstantiation(float delay);

        Texture *texture;
        float invencible_time; // 0.05s
        glm::vec4 current_color{1, 1, 1, 1};

    public:
        Enemy() : GameObject(){}
        ~Enemy();
        void Start() override;
        void Update() override;
        Player *player;
        float hitbox_radio = 0.125f;

        glm::vec3 head_pos{-0.11f, 0.3f, 0};
        glm::vec3 base_pos{3.07f, 0, 0};

        Material *bullet_material;
        ProjectileScene *currScene;

        void Shoot();
        void GenerateBullet(glm::vec3 dir, glm::vec3 acc, float speed, glm::vec3 pos);
        void ShootMultiDirection(float speed, glm::vec3 acc);
        void ShootMultiDirDelayed(float speed, float delay, glm::vec3 acc);
        void SimpleMove();
        Shader *shader;
        bool CollisionCircleCircle(glm::vec3 collider, float col_rad);
};
#endif