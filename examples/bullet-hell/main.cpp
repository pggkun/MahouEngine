#include <stdio.h>
#include <time.h>
#include <shader.h>
#include <game_entity.h>
#include <window.h>
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
    glewInit();
    glViewport(0, 0, WIDTH, HEIGHT);

    Shader text_shader(
        reinterpret_cast<const char*>(text_vert_shader), 
        reinterpret_cast<const char*>(text_frag_shader), nullptr);
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

    Enemy *enemy = new Enemy(nullptr, mainCam);
    enemy->Start();
    enemy->player = player;

    enemy->currScene = bulletScene;

    AnimatedSprite *sakura2 = new AnimatedSprite(1, 1, 0.1f, sakura1_png, sakura1_png_size);
    Transform *sakura_tr2 = new Transform();
    sakura_tr2->position = glm::vec3{8.0f, -1.6f, 2.5f};
    sakura_tr2->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    sakura_tr2->angle = 0.0f;
    sakura_tr2->scaleAmount = glm::vec3{5.0f, 5.0f, 1.0f};
    GameEntity *sakura_entity2 = new GameEntity(sakura2, mainCam);
    sakura_entity2->transform = sakura_tr2;
    sakura_entity2->Start();

    AnimatedSprite *bg = new AnimatedSprite(1, 1, 0.1f, background_png, background_png_size);
    Transform *bgt = new Transform();
    bgt->position = glm::vec3{0.0f, 0.0f, -2.0f};
    bgt->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    bgt->angle = 0.0f;
    bgt->scaleAmount = glm::vec3{12.0f, 12.0f, 1.0f};
    GameEntity *background = new GameEntity(bg, mainCam);
    background->transform = bgt;
    background->Start();

    Shader shader(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);

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

    TextRenderer *text1 = new TextRenderer("../resources/VT323-Regular.ttf", 26);
 
    Texture *texture = new Texture(dummytex_png, dummytex_png_size);
    Material *spriteDefault = new Material();
    spriteDefault->SetProperties(&shader, texture, mainCam, {1, 1, 1, 1});

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

        background->Update();
        background->Draw();

        red_bullets->SetupMatrices();
        red_bullets->Bind();
        red_bullets->Draw();

        bulletScene->Update();
        window->run();
        if (window->Fire1)
        {
            ProjectileObject *bullet;
            if (bulletScene->inactive_entities.size() > 0)
                bullet = bulletScene->GetFromInactive();
            bullet->Load(red_bullets, mainCam, glm::vec3{1, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0});
            bulletScene->Add(bullet);
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

        enemy->Update();
        player->Update();

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
        sakura_entity2->Update();

        enemy->Draw();
        player->Draw();
        player->DrawLife();

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
        
        text1->RenderText(text_shader, fpss.c_str(), 25.0f, 310.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
        text1->RenderText(text_shader, blts.c_str(), 25.0f, 290.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
    
        text1->RenderText(text_shader, mmr.c_str(), 25.0f, 270.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
        GameTime::draw_calls = 0;
        SDL_GL_SwapWindow(window->Window);
        uint64_t currTime = SDL_GetPerformanceCounter();
        GameTime::delta_time = static_cast<double>(
            (currTime - startTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
    }

    delete spriteDefault;
    delete player;
    delete enemy;
    delete sakura_entity;
    delete sakura_entity2;
    delete window;
    delete mainCam;
    delete text1;
    return 0;
}