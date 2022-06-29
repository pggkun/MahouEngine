#ifndef _TILE_H_
#define _TILE_H_
#include <game_object.h>

class Tile : public GameObject
{
    protected:
        glm::mat4 modelViewMatrix;

    public:
        Tile(Material *mat, Camera *cam, glm::vec3 pos, Texture *tex, int x, int y, int r, int c);
        ~Tile();
        void CustomUpdate();
        void RemoveMaterial();
};

#endif