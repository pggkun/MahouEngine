#include "enemy.h"
#include <monster1_png.h>
#include <bulet2_png.h>
#include <iostream>
#include <stdlib.h>
#define PI 3.14159265

int Enemy::global_count = 0;

void Enemy::Start()
{
    texture = new Texture(monster1_png, monster1_png_size);
    material = new Material();
    material->SetProperties(shader, texture, camera, {1, 1, 1, 1});

    this->transform = new Transform();
    this->transform->position = glm::vec3{3.2f, 1.0f, -0.1f};
    this->transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    this->transform->angle = 0.0f;
    this->transform->scaleAmount = glm::vec3{-1.5f, 1.5f, 1.0f};

    SetupPlane(0, 0, 1, 1);
    AssignMaterial(material);
    AssignSprite(4, 1);
    sprite->AddAnimation(std::vector<int>{0, 1, 2, 3});
    sprite->SwitchAnimation(1, 0.08f);

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
{
    if (invencible_time > 0)
        invencible_time -= GameTime::delta_time;
    for (std::list<ProjectileObject *>::iterator it = currScene->entities.begin(); it != currScene->entities.end(); ++it)
    {

        if (CollisionCircleCircle((*it)->transform->position, 0.125f) && invencible_time <= 0 && (*it)->from_player)
        {
            (*it)->RemoveMaterial();
            currScene->inactive_entities.push_back(*it);
            it = currScene->entities.erase(it);
            current_color = glm::vec4{1, 0, 0, 1};
            invencible_time = 0.05f;
            break;
        }
    }

    if (invencible_time <= 0)
    {
        current_color = glm::vec4{1, 1, 1, 1};
    }
    
    timer += GameTime::delta_time;
    if(timer >= 2.5f)
    {
        
        timer = 0.0;
        delay_timer = 0.0f;
        delay_counter = 0;

        ShootMultiDirection(1.5f, glm::vec3{(rand() % 2 + 0) - 1, (rand() % 2 + 0) - 1, 0});
    }

    if (this->bullet_layer >= 0.0505f + 0.001 * directions.size())
        this->bullet_layer = 0.0505f;
    
    shader->use();
    shader->setMat4("projMtx", camera->projMatrix());
    shader->setVec4("color", current_color);
    SimpleMove();
    this->BaseUpdate();
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
    bullet->Load(bullet_material, this->camera, dir * speed, acc, pos, false);
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

void Enemy::SimpleMove()
{
    // if(move_counter >= 360)
    //     move_counter = 0;
    // else
    //     move_counter += GameTime::delta_time;
    move_counter += GameTime::delta_time * 75.0f;
    float n_x = sin((move_counter * PI) / 180)/3.0f;
    float n_y = cos((move_counter * PI) / 180);
    transform->position = glm::vec3{n_x, n_y, 0} + base_pos;
}

bool Enemy::CollisionCircleCircle(glm::vec3 collider, float col_rad)
{
    glm::vec3 res_pos = transform->position + head_pos;
    return abs(((collider.x - res_pos.x) * (collider.x - res_pos.x)) + ((collider.y - res_pos.y) * (collider.y - res_pos.y))) < ((hitbox_radio + col_rad) * (hitbox_radio + col_rad));
}
