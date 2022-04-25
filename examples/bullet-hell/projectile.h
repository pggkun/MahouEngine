#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_
#include <game_entity.h>

class Projectile : public GameEntity
{
protected:
    glm::mat4 modelViewMatrix;
    void Move();
    float timer;

public:
    Projectile();
    ~Projectile();

    void Start() override;
    void Update() override;

    void Initialize();

    glm::vec3 origin;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    void CustomStart();
    void CustomUpdate();
    void CustomDraw();

    void ScaleMin();
    void ScaleMax();
    void Load(AnimatedSprite *spr, Camera *cam, glm::vec3 vel, glm::vec3 acc, glm::vec3 pos, const void *file, int len);
};

#endif