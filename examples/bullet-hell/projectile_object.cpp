#include "projectile_object.h"
#include <stdio.h>

ProjectileObject::ProjectileObject()
{

}

ProjectileObject::~ProjectileObject()
{
    RemoveMaterial();
    delete transform;
    delete sprite;
}

void ProjectileObject::RemoveMaterial()
{
    for (std::list<std::pair<Transform *, Mesh *>>::iterator it = material->objectsToDraw.begin(); it != material->objectsToDraw.end(); /**/)
    {
        if ((*it).second == mesh)
        {
            it = material->objectsToDraw.erase(it);
        }
        else
        {
            it = next(it,1);
        }
    }
    material = nullptr;
}

void ProjectileObject::Load(Material *mat, Camera *cam, glm::vec3 vel, glm::vec3 acc, glm::vec3 pos, bool f_player)
{
    this->camera = cam;

    transform->position = pos;
    transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    transform->angle = 0.0f;
    transform->scaleAmount = glm::vec3{0.175f, 0.175f, 1.0f};
    origin = pos;
    velocity = vel;
    acceleration = acc;
    timer = 0.0f;

    SetupPlane(0, 0, 1, 1);
    AssignMaterial(mat);
    from_player = f_player;
    //AssignSprite(1, 1);
}

void ProjectileObject::SoftLoad(glm::vec3 vel, glm::vec3 acc, glm::vec3 pos, bool f_player)
{
    transform->position = pos;
    transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    transform->angle = 0.0f;
    transform->scaleAmount = glm::vec3{0.175f, 0.175f, 1.0f};
    origin = pos;
    velocity = vel;
    acceleration = acc;
    timer = 0.0f;
    from_player = f_player;
}

void ProjectileObject::Initialize()
{
    transform = new Transform();
}

inline void ProjectileObject::Move()
{
    timer += GameTime::delta_time;
    float sx = origin.x + (velocity.x * timer) + (0.5f * acceleration.x * (timer * timer));
    float sy = origin.y + (velocity.y * timer) + (0.5f * acceleration.y * (timer * timer));
    transform->position = glm::vec3{sx, sy, transform->position.z};
}

void ProjectileObject::CustomUpdate()
{
    Move();
    modelViewMatrix = this->camera->viewMatrix() * this->transform->modelMatrix();
    mesh->modelViewMatrix = this->modelViewMatrix;
}

void ProjectileObject::Start()
{

}

void ProjectileObject::Update()
{
    //CustomUpdate();
}