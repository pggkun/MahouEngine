#include "enemy.h"
#include <monster1_png.h>
#include <bulet2_png.h>
#include <iostream>
#include <stdlib.h>
#define PI 3.14159265

int Enemy::global_count = 0;

void Enemy::Start()
{
    this->transform = new Transform();
    this->transform->position = glm::vec3{3.2f, 1.0f, -0.1f};
    this->transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    this->transform->angle = 0.0f;
    this->transform->scaleAmount = glm::vec3{-1.5f, 1.5f, 1.0f};

    this->sprite = new AnimatedSprite(4, 1, 0.08f, monster1_png, monster1_png_size);
    this->sprite->PushAnimation(std::vector<int>{0, 1, 2, 3});
    this->sprite->SwitchAnimation(1, 0.08f);

    this->interval = glm::vec2{0.0f, 360.0f};
    GetDirections(50);

    this->bullet_layer = 0.0505f;
    this->BaseStart();
    global_count++;
    this->self_count = global_count;
    timer = 0.0;
    delay_timer = 0.0f;
    delay_counter = 0;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{   timer += GameTime::delta_time;
    if(timer >= 10.0f)
    {
        
        timer = 0.0;
        delay_timer = 0.0f;
        delay_counter = 0;

        ShootMultiDirection(0.05f,glm::vec3{0,0,0});// glm::vec3{(rand() % 2 + 0) - 1, (rand() % 2 + 0)  -1 ,0});
    }

    if (this->bullet_layer >= 0.0505f + 0.001 * directions.size())
        this->bullet_layer = 0.0505f;
    
    this->sprite->UpdateAnimation();
    this->BaseUpdate();
}

void Enemy::SimpleMoveDown()
{
    if (this->coroutine == nullptr)
    {
        this->coroutine = (void(GameEntity::*)()) & SimpleMoveDown;
    }
    if (this->transform->position.y > -1.0f)
    {
        this->transform->position.y -= 1.0f * GameTime::delta_time;
    }
    else
    {
        this->coroutine = (void(GameEntity::*)()) & SimpleMoveUp;
    }
}

void Enemy::SimpleMoveUp()
{
    if (this->coroutine == nullptr)
    {
        this->coroutine = (void(GameEntity::*)()) & SimpleMoveUp;
    }
    if (this->transform->position.y < 1.0f)
    {
        this->transform->position.y += 1.0f * GameTime::delta_time;
    }
    else
    {
        this->coroutine = (void(GameEntity::*)()) & SimpleMoveDown;
    }
}

void Enemy::Shoot()
{
    float x_offset = -0.2f;
    glm::vec3 new_pos = glm::vec3{transform->position.x + x_offset, transform->position.y, 0.0f + this->bullet_layer};
    glm::vec3 dir = glm::normalize(player->transform->position - this->transform->position);
    glm::vec3 acc = -(dir*0.3f);
    GenerateBullet(dir, acc, 2.0f, new_pos);
}

void Enemy::GenerateBullet(glm::vec3 dir, glm::vec3 acc, float speed, glm::vec3 pos)
{
    ProjectileObject *bullet;
    if (currScene->inactive_entities.size() > 0)
        bullet = currScene->GetFromInactive();
    else
        bullet = (ProjectileObject *)currScene->memory_pool.New();
    bullet->Load(bullet_material, this->camera, dir * speed, acc, pos);
    currScene->Add(bullet);

    this->bullet_layer += 0.001f;
}

void Enemy::ShootMultiDirection(float speed, glm::vec3 acc)
{
    float x_offset = -0.2f;
    glm::vec3 new_pos = glm::vec3{transform->position.x + x_offset, transform->position.y, 0.0f + this->bullet_layer};

    for (glm::vec3 d : directions)
    {
        GenerateBullet(d, acc, speed, new_pos);
    }
}

void Enemy::ShootMultiDirDelayed(float speed, float delay, glm::vec3 acc)
{
    float x_offset = -0.2f;
    glm::vec3 new_pos = glm::vec3{transform->position.x + x_offset, transform->position.y, 0.0f + this->bullet_layer};
    std::vector<glm::vec3> dir(directions.begin(), directions.end());
    if (delay_timer > delay * delay_counter && delay_counter < directions.size())
    {
        GenerateBullet(dir[delay_counter], acc, speed, new_pos);
        delay_counter++;
    }
    delay_timer += GameTime::delta_time;
}

void Enemy::GetDirections(float amount)
{
    directions.clear();
    for (int i = 0; i < amount; i++)
    {
        float angle = (360.0f / (float)amount) * i;
        if ((angle < interval.x) || (angle > interval.y))
            continue;
        float sin_angle = sin((angle * PI) / 180);
        float cos_angle = cos((angle * PI) / 180);
        directions.push_back(glm::vec3{cos_angle, sin_angle, 0.0f});
    }
}
