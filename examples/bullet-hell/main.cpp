#include <stdio.h>
#include <time.h>
#include <gl/glcorearb.h>
#include <gl/glext.h>
#include <game_entity.h>
#include <window.h>
#include <scene.h>
#include <text_renderer.h>

#include <material.h>
#include <mesh.h>
#include <texture.h>
#include <game_object.h>

#include <iostream>
#include <algorithm>
#include <string>

#include "player.h"
#include "enemy.h"
#include "projectile_object.h"

#include <sakura1_png.h>
#include <background_png.h>
#include <dummytex_png.h>
#include <monster1_png.h>
#include <bulet2_png.h>
#include <bullet1_png.h>
#include <bichin_png.h>
#include <lua_object.h>

#define WINDOW_TITLE "Mahou Engine - Bullet Hell"
#define WIDTH 640
#define HEIGHT 360

constexpr auto TAU = glm::two_pi<float>();

void CheckGL()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        printf("\n A general  OGL error has occured: %i\n", err);
    }
}

int main(int ArgCount, char **Args)
{
    Window *window = new Window;
    window->init(WINDOW_TITLE, WIDTH, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);

    Shader text_shader(textVertexShaderSource, textFragmentShaderSource, nullptr);
    text_shader.use();

    Camera *mainCam = new Camera();
    mainCam->aperture = 45.0f * TAU / 360.0f;
    mainCam->ratio = 1280.0f / 720.0f;
    mainCam->cnear = 0.01f;
    mainCam->cfar = 1000.0f;
    mainCam->eye = glm::vec3{0.0f, 0.0f, 6.0f};
    mainCam->center = glm::vec3{0.0f, 0.0f, 0.0f};
    mainCam->up = glm::vec3{0.0f, 1.0f, 0.0f};

    Player *player = new Player(nullptr, mainCam);
    player->Start();

    PoolScene *mainScene = new PoolScene();
    player->currentScene = mainScene;

    ProjectileScene *bulletScene = new ProjectileScene();
    player->currScene = bulletScene;
    
    AnimatedSprite *sakura = new AnimatedSprite(1, 1, 0.1f, sakura1_png, sakura1_png_size);
    Transform *sakura_tr = new Transform();
    sakura_tr->position = glm::vec3{6.0f, -1.0f, 1.0f};
    sakura_tr->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    sakura_tr->angle = 0.0f;
    sakura_tr->scaleAmount = glm::vec3{5.0f, 5.0f, 1.0f};
    GameEntity *sakura_entity = new GameEntity(sakura, mainCam);
    sakura_entity->transform = sakura_tr;
    sakura_entity->Start();
    sakura_entity->sprite->sprite_color = glm::vec4{0.9f, 0.9f, 0.9f, 1.0f};
    //==========================================

    Enemy *enemy = new Enemy(nullptr, mainCam);
    enemy->Start();
    enemy->player = player;
    enemy->currentScene = mainScene;

    enemy->currScene = bulletScene;
    //==========================================

    //==========================================
    AnimatedSprite *sakura2 = new AnimatedSprite(1, 1, 0.1f, sakura1_png, sakura1_png_size);
    Transform *sakura_tr2 = new Transform();
    sakura_tr2->position = glm::vec3{8.0f, -1.6f, 2.5f};
    sakura_tr2->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    sakura_tr2->angle = 0.0f;
    sakura_tr2->scaleAmount = glm::vec3{5.0f, 5.0f, 1.0f};
    GameEntity *sakura_entity2 = new GameEntity(sakura2, mainCam);
    sakura_entity2->transform = sakura_tr2;
    sakura_entity2->Start();
    //==========================================

    //==========================================
    AnimatedSprite *bg = new AnimatedSprite(1, 1, 0.1f, background_png, background_png_size);
    Transform *bgt = new Transform();
    bgt->position = glm::vec3{0.0f, 0.0f, -2.0f};
    bgt->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    bgt->angle = 0.0f;
    bgt->scaleAmount = glm::vec3{12.0f, 12.0f, 1.0f};
    GameEntity *background = new GameEntity(bg, mainCam);
    background->transform = bgt;
    background->Start();
    //==========================================
    /*AnimatedSprite *b2 = new AnimatedSprite(1, 1, 0.1f, bulet2_png, bulet2_png_size);
    Transform *b2t = new Transform();
    b2t->position = glm::vec3{0.0f, 0.0f, 0.0f};
    b2t->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    b2t->angle = 0.0f;
    b2t->scaleAmount = glm::vec3{0.2f, 0.2f, 1.0f};
    GameEntity *life = new GameEntity(b2, mainCam);
    b2->sprite_color = glm::vec4{1.0f, 0.7f, 0.7f, 1};
    life->transform = b2t;
    life->Start();*/
    //==========================================
    /*for (int i = 0; i < 500; i++)
    {
        Projectile *np = (Projectile *)mainScene->memory_pool.New();
        np->Initialize();
        mainScene->inactive_entities.push_back(np);
    }*/

    Shader shader(vs, fs, nullptr);

    Texture *bullet_red = new Texture(bulet2_png, bulet2_png_size);
    Material *red_bullets = new Material;
    red_bullets->SetProperties(&shader, bullet_red, mainCam, {1, 1, 1, 1});

    enemy->bullet_material = red_bullets;

    for (int i = 0; i < 800; i++)
    {
        ProjectileObject *np = (ProjectileObject *)bulletScene->memory_pool.New();
        np->Initialize();
        bulletScene->inactive_entities.push_back(np);
    }

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_ALWAYS);

    //==========================================
    TextRenderer *text1 = new TextRenderer("../resources/VT323-Regular.ttf", 26);
    //GameTime *tm = new GameTime();
    //==========================================

    
    Texture *texture = new Texture(dummytex_png, dummytex_png_size);
    Material *spriteDefault = new Material();
    spriteDefault->SetProperties(&shader, texture, mainCam, {1, 1, 1, 1});

    /*std::vector<GameObject *> bgscene;
    for(int i = 0; i<100; i++) 
    {
        GameObject *simplePlane = new GameObject();
        simplePlane->camera = mainCam;
        simplePlane->SetupPlane(0,0,1,1);
        simplePlane->AssignMaterial(spriteDefault);
        simplePlane->AssignSprite(1, 1);

        int x = i%10;
        int y = i/10;
        simplePlane->MoveTo(x-5, y-5);
        bgscene.push_back(simplePlane);
    }*/

    /*GameObject *simplePlane = new GameObject();
    simplePlane->camera = mainCam;
    simplePlane->SetupPlane();

    GameObject *simplePlane2 = new GameObject();
    simplePlane2->camera = mainCam;
    simplePlane2->SetupPlane();

    
    simplePlane->AssignMaterial(spriteDefault);
    simplePlane2->AssignMaterial(spriteDefault);*/

    window->Fire1 = false;
    window->Fire2 = false;
    double delta_time;

    glEnable(GL_DEBUG_OUTPUT);
    while (window->Running)
    {
        uint64_t startTime = SDL_GetPerformanceCounter();
        SDL_GL_SetSwapInterval(0);
        glClearColor(106.0F / 255.0F, 102.0F / 255.0F, 125.0F / 255.0F, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("projMtx", mainCam->projMatrix());
        shader.setVec4("color", {1, 1, 1, 1});
        //simplePlane->Update();
        //simplePlane2->MoveTo(1,0, 0.2f);
        //simplePlane2->Update();
        /*for (int i = 0; i < 100; i++)
        {
            bgscene[i]->Update();
        }*/
        //bgscene[0]->sprite->Update();
        //spriteDefault->SetupMatrices();
        //spriteDefault->Draw();

        
        //printf("update scene\n");
        background->Update();
        background->Draw();

        red_bullets->SetupMatrices();
        //printf("setup matrices: ");
        //CheckGL();
        red_bullets->Bind();
        //printf("bind matrices: ");
        //CheckGL();
        red_bullets->Draw();
        //printf("draw instanced: ");
        //CheckGL();
        bulletScene->Update();
        window->run();
        if (window->Fire1)
        {
            ProjectileObject *bullet;
            if (bulletScene->inactive_entities.size() > 0)
                bullet = bulletScene->GetFromInactive();
            //else
            //    bullet = (ProjectileObject *)bulletScene->memory_pool.New();
            bullet->Load(red_bullets, mainCam, glm::vec3{1, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0});
            bulletScene->Add(bullet);
            //printf("adicionai\n");
        }

        if (window->Fire2)
        {
            player->ShootPattern1();
        }

        if (window->Shift)
        {
            player->focus = true;
        }
        else
        {
            player->focus = false;
        }
              
        player->x_input = window->xOff;
        player->y_input = window->zOff;
        
        //background->Draw();

        enemy->Update();

        player->Update();
        //player->Draw();

        //life->transform->position = player->transform->position;
        //life->transform->position.z = 0.1f;

        /*
        if (player->focus)
            life->Draw();*/
     
        //mainScene->Update();
        //mainScene->Draw();

        sakura_entity->transform->position.x -= 2.5f * GameTime::delta_time;
        if(sakura_entity->transform->position.x <= -6.0f)
        {
            sakura_entity->transform->position.x = 6.0f;
        }

        sakura_entity2->transform->position.x -= 2.5f * GameTime::delta_time;
        if (sakura_entity2->transform->position.x <= -6.0f)
        {
            sakura_entity2->transform->position.x = 8.0f;
        }

        sakura_entity->Update();
        //sakura_entity->Draw();
        sakura_entity2->Update();
        //sakura_entity2->Draw();
        
        enemy->Draw();
        player->Draw();
        player->DrawLife();
        //mainScene->Draw();
        sakura_entity->Draw();
        sakura_entity2->Draw();

        window->Fire1 = false;
        window->Fire2 = false;
        window->xOff = 0.0f;
        window->zOff = 0.0f;
        std::string mmr = "Draw Calls: " + std::to_string(GameTime::draw_calls);
        std::string playerhit = "player hp: " + std::to_string(player->hp);
        std::string fpss = std::to_string((int)(1.0 / GameTime::delta_time)) + " FPS";
        std::string blts = "Active Bullets: " + std::to_string(bulletScene->entities.size()) + " Inactive Bullets: " + std::to_string(bulletScene->inactive_entities.size());
        
        //CheckGL();
        //text1->RenderText(text_shader, playerhit.c_str(), 25.0f, 330.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
        text1->RenderText(text_shader, fpss.c_str(), 25.0f, 310.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
        text1->RenderText(text_shader, blts.c_str(), 25.0f, 290.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));

        
        text1->RenderText(text_shader, mmr.c_str(), 25.0f, 270.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
        GameTime::draw_calls = 0;
        //SDL_RenderPresent(window->renderer);
        SDL_GL_SwapWindow(window->Window);
        uint64_t currTime = SDL_GetPerformanceCounter();
        GameTime::delta_time = static_cast<double>(
            (currTime - startTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
    }
    /*for(int i = 0; i<100; i++)
    {
        delete bgscene[i];
    }*/
    delete spriteDefault;
    delete player;
    delete enemy;
    delete sakura_entity;
    delete sakura_entity2;
    delete window;
    delete mainCam;
    delete mainScene;
    delete text1;
    return 0;
}