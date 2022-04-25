#include "pool_scene.h"
#include <stdio.h>

PoolScene::PoolScene()
{
}

PoolScene::~PoolScene()
{
    for (auto i : entities)
        memory_pool.Delete(i);
}

void PoolScene::Add(Projectile *obj)
{
    entities.push_back(obj);
    obj->CustomStart();
}

void PoolScene::Update()
{
    next = entities.begin();
    while (next != entities.end())
    {
        it = next++;
        (*it)->CustomUpdate();
        if ((*it)->transform->position.x > 5.0f
        ||(*it)->transform->position.x < -5.0f
        || (*it)->transform->position.y > 4.0f
        || (*it)->transform->position.y < -4.0f)
        {
            inactive_entities.push_back(*it);
            entities.erase(it);
        }
    }
}

Projectile * PoolScene::GetFromInactive()
{
    Projectile *inactive = inactive_entities.front();
    if (inactive)inactive_entities.erase(inactive_entities.begin());
    return inactive;
}

void PoolScene::Draw()
{
    next = entities.begin();
    while (next != entities.end())
    {
        it = next++;
        (*it)->CustomDraw();
    }
}

void PoolScene::Begin()
{
    next = entities.begin();
}

Projectile *PoolScene::Next()
{
    if (next != entities.end())
    {
        it = next++;
        return *it;
    }
    else
        return nullptr;
}

void PoolScene::Remove()
{
    memory_pool.Delete(*it);
    entities.erase(it);
}