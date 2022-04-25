#include "game_entity.h"
#include <stdio.h>

GameEntity::GameEntity(AnimatedSprite *spr, Camera *cam)
{
    this->transform = new Transform();
    this->sprite = spr;
    this->camera = cam;
}

GameEntity::~GameEntity()
{
    delete this->transform;
}

void GameEntity::BaseStart()
{
    glUniformMatrix4fv(this->sprite->loc_projMtx, 1, GL_FALSE, glm::value_ptr(camera->projMatrix()));
}

void GameEntity::BaseUpdate()
{
    if(coroutine != nullptr)
    {
        (*this.*coroutine)();
    }
}

void GameEntity::Start()
{
    this->BaseStart();
}

void GameEntity::Update()
{
    this->BaseUpdate();
}

void GameEntity::Draw()
{
    glUseProgram(this->sprite->s_program);
    this->modelViewMatrix = this->camera->viewMatrix() * this->transform->modelMatrix();
    glUniformMatrix4fv(this->sprite->loc_mdlvMtx, 1, GL_FALSE, glm::value_ptr(this->modelViewMatrix));
    this->sprite->Draw();
}