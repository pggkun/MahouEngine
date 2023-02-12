#ifndef _PROJECTILE_OBJECT_H_
#define _PROJECTILE_OBJECT_H_
#include <game_object.h>

class ProjectileObject : public GameObject
{
protected:
    glm::mat4 modelViewMatrix;
    void Move();
    float timer;

public:
    ProjectileObject();
    ~ProjectileObject();

    void Start() override;
    void Update() override;

    void Initialize();

    glm::vec3 origin;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    void CustomStart();
    void CustomUpdate();
    void CustomDraw();

    void RemoveMaterial();

    bool from_player;

    void Load(Material *mat, Camera *cam, glm::vec3 vel, glm::vec3 acc, glm::vec3 pos, bool f_player);
    void SoftLoad(glm::vec3 vel, glm::vec3 acc, glm::vec3 pos, bool f_player);
};

#endif