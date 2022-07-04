#include "projectile_scene.h"
#include <stdio.h>

ProjectileScene::ProjectileScene()
{
}

ProjectileScene::~ProjectileScene()
{
    for (auto i : entities)
        memory_pool.Delete(i);
}

void ProjectileScene::Add(ProjectileObject *obj)
{
    entities.push_back(obj);
    //obj->Start();
}

void ProjectileScene::Update()
{
    prox = entities.begin();
    while (prox != entities.end())
    {
        it = prox++;
        (*it)->CustomUpdate();
        if ((*it)->transform->position.x > 5.0f || (*it)->transform->position.x < -5.0f || (*it)->transform->position.y > 4.0f || (*it)->transform->position.y < -4.0f)
        {
            (*it)->RemoveMaterial();
            inactive_entities.push_back(*it);
            entities.erase(it);
        }
    }
}

ProjectileObject *ProjectileScene::GetFromInactive()
{
    ProjectileObject *inactive = inactive_entities.front();
    if (inactive)
        inactive_entities.erase(inactive_entities.begin());
    return inactive;
}

inline void ProjectileScene::Begin()
{
    prox = entities.begin();
}

inline ProjectileObject *ProjectileScene::Next()
{
    if (prox != entities.end())
    {
        it = prox++;
        return *it;
    }
    else
        return nullptr;
}