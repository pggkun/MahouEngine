#include "player.h"
#include "projectile.h"
#include <sakfly_png.h>
#include <bullet1_png.h>
#include <bulet2_png.h>
#include <cmath>

int Player::global_count = 0;

void Player::Start()
{
    this->transform = new Transform();
    this->transform->position = glm::vec3{-3.0f, 0.0f, 0.0f};
    this->transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    this->transform->angle = 0.0f;
    this->transform->scaleAmount = glm::vec3{2.5f, 2.5f, 1.0f};

    this->sprite = new AnimatedSprite(4, 1, 0.08f, sakfly_png, sakfly_png_size);
    this->sprite->PushAnimation(std::vector<int>{0, 1, 2, 3});
    this->sprite->SwitchAnimation(1, 0.08f);
    this->bullet_layer = 0.001f;
    this->BaseStart();
    global_count++;
    this->self_count = global_count;

    AnimatedSprite *b2 = new AnimatedSprite(1, 1, 0.1f, bulet2_png, bulet2_png_size);
    Transform *b2t = new Transform();
    b2t->position = glm::vec3{0.0f, 0.0f, 0.0f};
    b2t->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    b2t->angle = 0.0f;
    b2t->scaleAmount = glm::vec3{0.2f, 0.2f, 1.0f};
    this->life_vfx = new GameEntity(b2, this->camera);
    b2->sprite_color = glm::vec4{1.0f, 0.7f, 0.7f, 1};
    this->life_vfx->transform = b2t;
    this->life_vfx->Start();

    hp = 100;
    hitbox_radio = 0.085f;
}

Player::~Player()
{
    delete transform;
    delete sprite;
}

void Player::Move(float x, float y, float z, float speed)
{
    float s = speed;
    if(focus)
        s = speed/2.0f;
    this->transform->position = glm::vec3{this->transform->position.x + (x * s),
                                          this->transform->position.y + (y * s),
                                          this->transform->position.z + (z * s)};
}

void Player::Update()
{

    for (std::list<ProjectileObject *>::iterator it = currScene->entities.begin(); it != currScene->entities.end(); ++it)
    {
        
        if (CollisionCircleCircle((*it)->transform->position, 0.125f))
        {
            hp -= 1;
            (*it)->RemoveMaterial();
            currScene->inactive_entities.push_back(*it);
            currScene->entities.erase(it);
        }
    }


    this->sprite->UpdateAnimation();
    if(this->bullet_layer >= 0.05f) this->bullet_layer = 0.0f;
    Move(-x_input, y_input, 0.0f, 5.0f * GameTime::delta_time);
    this->BaseUpdate();

    life_vfx->transform->position = this->transform->position;
    this->life_vfx->Update();
    
}

void Player::DrawLife()
{
    if (focus)
        this->life_vfx->Draw();
}

void Player::Shoot()
{
    float x_offset = 0.6f;
    glm::vec3 new_pos = glm::vec3{transform->position.x + x_offset, transform->position.y, transform->position.z + this->bullet_layer};
    glm::vec3 acc = glm::vec3{0,-10,0};
    glm::vec3 vel = glm::vec3{5, 5, 0};
    Projectile *bullet;
    if (currentScene->inactive_entities.size() > 0)
        bullet = currentScene->GetFromInactive();
    else
        bullet = (Projectile *)currentScene->memory_pool.New();
    bullet->Load(nullptr, this->camera, vel, acc, new_pos, bullet1_png, bullet1_png_size);
    currentScene->Add(bullet);
    //bullet->current_scene = this->currentScene;
    this->bullet_layer += 0.001f;
}

void Player::ShootPattern1()
{
    float x_offset = 0.6f;
    glm::vec3 new_pos = glm::vec3{transform->position.x + x_offset, transform->position.y, transform->position.z + this->bullet_layer};
    glm::vec3 acc = glm::vec3{-6, 0, 0};
    glm::vec3 vel = glm::vec3{3, 0, 0};
    Projectile *bullet;
    if (currentScene->inactive_entities.size() > 0)
        bullet = currentScene->GetFromInactive();
    else
        bullet = (Projectile *)currentScene->memory_pool.New();
    bullet->Load(nullptr, this->camera, vel, acc, new_pos, bullet1_png, bullet1_png_size);
    currentScene->Add(bullet);
    //bullet->current_scene = this->currentScene;
    this->bullet_layer += 0.001f;

    new_pos = glm::vec3{transform->position.x + x_offset, transform->position.y, transform->position.z + this->bullet_layer};
    acc = glm::vec3{6, 0, 0};
    vel = glm::vec3{-3, 0, 0};
    Projectile *bullet2;
    if (currentScene->inactive_entities.size() > 0)
        bullet2 = currentScene->GetFromInactive();
    else
        bullet2 = (Projectile *)currentScene->memory_pool.New();
    bullet2->Load(nullptr, this->camera, vel, acc, new_pos, bullet1_png, bullet1_png_size);
    currentScene->Add(bullet2);
    //bullet2->current_scene = this->currentScene;
    this->bullet_layer += 0.001f;

    new_pos = glm::vec3{transform->position.x + x_offset, transform->position.y, transform->position.z + this->bullet_layer};
    acc = glm::vec3{0, -6, 0};
    vel = glm::vec3{0, 3, 0};
    Projectile *bullet3;
    if (currentScene->inactive_entities.size() > 0)
        bullet3 = currentScene->GetFromInactive();
    else
        bullet3 = (Projectile *)currentScene->memory_pool.New();
    bullet3->Load(nullptr, this->camera, vel, acc, new_pos, bullet1_png, bullet1_png_size);
    currentScene->Add(bullet3);
    //bullet3->current_scene = this->currentScene;
    this->bullet_layer += 0.001f;

    new_pos = glm::vec3{transform->position.x + x_offset, transform->position.y, transform->position.z + this->bullet_layer};
    acc = glm::vec3{0, 6, 0};
    vel = glm::vec3{0, -3, 0};
    Projectile *bullet4;
    if (currentScene->inactive_entities.size() > 0)
        bullet4 = currentScene->GetFromInactive();
    else
        bullet4 = (Projectile *)currentScene->memory_pool.New();
    bullet4->Load(nullptr, this->camera, vel, acc, new_pos, bullet1_png, bullet1_png_size);
    currentScene->Add(bullet4);
    //bullet4->current_scene = this->currentScene;
    this->bullet_layer += 0.001f;
}

void Player::FadeOut()
{
    if(this->coroutine == nullptr)
    {
        this->coroutine = (void(GameEntity::*)()) &FadeOut;
    }
    if (this->sprite->sprite_color.a > 0.0f)
    {
        this->sprite->sprite_color = glm::vec4{1, 1, 1, sprite->sprite_color.a - 0.07f};
    }
    else
    {
        this->coroutine = nullptr;
    }
}

bool Player::CollisionCircleCircle(glm::vec3 collider, float col_rad)
{
    return abs(((collider.x - transform->position.x) * (collider.x - transform->position.x)) + ((collider.y - transform->position.y) * (collider.y - transform->position.y)) ) < ((hitbox_radio + col_rad) * (hitbox_radio + col_rad));
}
 

void Player::FadeIn()
{
    if (this->coroutine == nullptr)
    {
        this->coroutine = (void(GameEntity::*)()) &FadeIn;
    }

    if (this->sprite->sprite_color.a < 1.0f)
    {
        this->sprite->sprite_color = glm::vec4{1, 1, 1, sprite->sprite_color.a + 0.07f};
    }
    else
    {
        this->coroutine = nullptr;
    }
}