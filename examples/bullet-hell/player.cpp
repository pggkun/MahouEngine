#include "player.h"
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

    texture = new Texture(sakfly_png, sakfly_png_size);
    material = new Material();
    material->SetProperties(shader, texture, camera, {1, 1, 1, 1});

    SetupPlane(0, 0, 1, 1);
    AssignMaterial(material);
    AssignSprite(4, 1);
    sprite->AddAnimation(std::vector<int>{0, 1, 2, 3});
    sprite->SwitchAnimation(1, 0.08f);

    global_count++;
    this->self_count = global_count;

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

    this->transform->position = glm::vec3{(std::clamp(this->transform->position.x, -3.68f, 3.76f)),
                                          (std::clamp(this->transform->position.y, -2.11f, 1.83f)),
                                          this->transform->position.z};
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
            it = currScene->entities.erase(it);
            break;
        }
    }

    shader->use();
    shader->setMat4("projMtx", camera->projMatrix());
    shader->setVec4("color", {1, 1, 1, 1});

    if(this->bullet_layer >= 0.05f) this->bullet_layer = 0.0f;
    Move(-x_input, y_input, 0.0f, 5.0f * GameTime::delta_time);
    this->BaseUpdate();
    
}

void Player::DrawLife()
{

}

bool Player::CollisionCircleCircle(glm::vec3 collider, float col_rad)
{
    return abs(((collider.x - transform->position.x) * (collider.x - transform->position.x)) + ((collider.y - transform->position.y) * (collider.y - transform->position.y)) ) < ((hitbox_radio + col_rad) * (hitbox_radio + col_rad));
}