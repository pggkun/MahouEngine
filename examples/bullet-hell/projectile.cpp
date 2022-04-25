#include "projectile.h"
#include <bullet1_png.h>
#include <stdio.h>

void Projectile::Load(AnimatedSprite *spr, Camera *cam, glm::vec3 vel, glm::vec3 acc, glm::vec3 pos, const void *file, int len)
{
    this->camera = cam;
    
    transform->position = pos;
    transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    transform->angle = 0.0f;
    transform->scaleAmount = glm::vec3{0.25f, 0.25f, 1.0f};
    origin = pos;
    velocity = vel;
    acceleration = acc;
    timer = 0.0f;
    this->sprite = new AnimatedSprite(1, 1, 0.06f, file, len);
}


Projectile::~Projectile()
{
    delete transform;
    delete sprite;
}

void Projectile::Initialize()
{
    this->transform = new Transform();
}

void Projectile::Start()
{

}

void Projectile::CustomStart()
{
    //ScaleMin();
    GameEntity::BaseStart();
}

void Projectile::CustomDraw()
{
    GameEntity::Draw();
}

void Projectile::Move()
{
    this->timer += GameTime::delta_time;
    float sx = origin.x + (velocity.x * timer) + (0.5f * acceleration.x * (timer * timer));
    float sy = origin.y + (velocity.y * timer) + (0.5f * acceleration.y * (timer * timer));
    this->transform->position = glm::vec3{sx, sy, transform->position.z};
}

void Projectile::Update()
{

}

void Projectile::CustomUpdate()
{
    Move();
    GameEntity::BaseUpdate();
}

void Projectile::ScaleMin()
{
    if (this->coroutine == nullptr)
    {
        this->coroutine = (void(GameEntity::*)()) & ScaleMin;
    }
    if (this->transform->scaleAmount.x > 0.20f)
    {
        this->transform->scaleAmount = glm::vec3{transform->scaleAmount.x - 0.3f * GameTime::delta_time, transform->scaleAmount.y - 0.3f * GameTime::delta_time, 1.0f};
    }
    else
    {
        this->coroutine = (void(GameEntity::*)()) & ScaleMax;
    }
}

void Projectile::ScaleMax()
{
    if (this->coroutine == nullptr)
    {
        this->coroutine = (void(GameEntity::*)()) & ScaleMax;
    }
    if (this->transform->scaleAmount.x < 0.25f)
    {
        this->transform->scaleAmount = glm::vec3{transform->scaleAmount.x + 0.3f * GameTime::delta_time, transform->scaleAmount.y + 0.3f * GameTime::delta_time, 1.0f};
    }
    else
    {
        this->coroutine = (void(GameEntity::*)()) & ScaleMin;
    }
}