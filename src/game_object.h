#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <gl/glcorearb.h>
#include <gl/glext.h>
#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>

#include "transform.h"
#include "camera.h"
#include <list>
#include "render/material.h"
#include "render/sprite.h"

class GameObject
{
    protected:
        glm::mat4 modelViewMatrix;

    public:
        GameObject();
        ~GameObject();

        Transform *transform;
        Camera *camera;
        Mesh *mesh;
        Sprite *sprite;
        Material *material;
        
        void MoveTo(float x, float y);
        void MoveTo(float x, float y, float z);

        void BaseStart();
        void BaseUpdate();
        
        virtual void Start();
        virtual void Update();

        void SetupPlane(float x, float y, float w, float h);

        void AssignMaterial(Material *m);

        void AssignSprite(float c, float r);

        /*template <class T>
        T *GetComponent(T);

        template <class T>
        void AddComponent(T);

        template <class T>
        void RemoveComponent(T);

        std::list<Component *> components;*/
};

#endif