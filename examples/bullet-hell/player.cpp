#include "player.h"
#include <sakfly_png.h>
#include <bullet1_png.h>
#include <bulet2_png.h>
#include <cmath>

int Player::global_count = 0;

Player::~Player()
{
    delete material;
    delete texture;
    delete life_texture;
    delete life_vfx;
}

void Player::Start()
{
    this->transform = new Transform();
    this->transform->position = glm::vec3{-3.0f, 0.0f, 0.0f};
    this->transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    this->transform->angle = 0.0f;
    this->transform->scaleAmount = glm::vec3{2.5f, 2.5f, 1.0f};

    texture = new Texture(sakfly_png, sakfly_png_size);
    material = new Material();
    material->SetProperties(shader, texture, camera, {1, 1, 1, 1});

    life_vfx = new GameObject();
    life_vfx->transform = new Transform();
    life_vfx->transform->position = glm::vec3{0.0f, 0.0f, 0.0f};
    life_vfx->transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    life_vfx->transform->angle = 0.0f;
    life_vfx->transform->scaleAmount = glm::vec3{0.175f, 0.175f, 1.0f};

    life_texture = new Texture(bulet2_png, bulet2_png_size);
    life_vfx->material = new Material();
    life_vfx->camera = camera;
    life_vfx->material->SetProperties(shader, life_texture, life_vfx->camera, {1, 1, 1, 1});
    life_vfx->SetupPlane(0, 0, 1, 1);
    life_vfx->AssignMaterial(life_vfx->material);
    life_vfx->AssignSprite(1, 1);

    SetupPlane(0, 0, 1, 1);
    AssignMaterial(material);
    AssignSprite(4, 1);
    sprite->AddAnimation(std::vector<int>{0, 1, 2, 3});
    sprite->SwitchAnimation(1, 0.08f);

    global_count++;
    this->self_count = global_count;

    hp = 100;
    hitbox_radio = 0.175f / 2.0f;
}

void Player::Move(float x, float y, float z, float speed)
{
    float s = speed;
    if(focus)
        s = speed/2.0f;
    this->transform->position = glm::vec3{this->transform->position.x + (x * s),
                                          this->transform->position.y + (y * s),
                                          this->transform->position.z + (z * s)};

    this->transform->position = glm::vec3{(std::clamp(this->transform->position.x, -3.68f, 3.76f)),
                                          (std::clamp(this->transform->position.y, -2.11f, 1.83f)),
                                          this->transform->position.z};
}

void Player::Update()
{
    if(invencible_time > 0)
        invencible_time -= GameTime::delta_time;
    for (std::list<ProjectileObject *>::iterator it = currScene->entities.begin(); it != currScene->entities.end(); ++it)
    {
        
        if (CollisionCircleCircle((*it)->transform->position, 0.125f) && invencible_time <= 0 && !(*it)->from_player)
        {
            hp -= 1;
            (*it)->RemoveMaterial();
            currScene->inactive_entities.push_back(*it);
            it = currScene->entities.erase(it);
            current_color = glm::vec4{1,0,0,1};
            invencible_time = 0.05f;
            break;
        }
    }

    if(invencible_time <= 0)
    {
        current_color = glm::vec4{1, 1, 1, 1};
    }

    shader->use();
    shader->setMat4("projMtx", camera->projMatrix());
    shader->setVec4("color", current_color);

    if(this->bullet_layer >= 0.05f) this->bullet_layer = 0.0f;
    Move(-x_input, y_input, 0.0f, 5.0f * GameTime::delta_time);
    this->BaseUpdate();

    life_vfx->transform->position = this->transform->position;
    this->life_vfx->Update();
}

void Player::DrawLife()
{
    if (focus)
    {
        life_vfx->material->SetupMatrices();
        life_vfx->material->Draw();
    }
}

bool Player::CollisionCircleCircle(glm::vec3 collider, float col_rad)
{
    return abs(((collider.x - transform->position.x) * (collider.x - transform->position.x)) + ((collider.y - transform->position.y) * (collider.y - transform->position.y)) ) < ((hitbox_radio + col_rad) * (hitbox_radio + col_rad));
}