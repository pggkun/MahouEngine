#include "scene.h"
#include <stdio.h>

Scene::Scene()
{
}

Scene::~Scene()
{
    for (auto i : entities)
        delete i;
}

void Scene::Add(GameEntity *obj)
{
    entities.push_back(obj);
    obj->Start();
}

void Scene::Update()
{
    next = entities.begin();
    while (next != entities.end())
    {
        it = next++;
        (*it)->Update();
    }
}

void Scene::Draw()
{
    next = entities.begin();
    while (next != entities.end())
    {
        it = next++;
        (*it)->Draw();
    }
}

void Scene::Begin()
{
    next = entities.begin();
}

GameEntity *Scene::Next()
{
    if (next != entities.end())
    {
        it = next++;
        return *it;
    }
    else
        return nullptr;
}

void Scene::Remove()
{
    delete (*it);
    entities.erase(it);
}