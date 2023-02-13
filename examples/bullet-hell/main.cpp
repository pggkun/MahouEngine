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
#include <monster1_png.h>
#include <bulet2_png.h>
#include <bullet1_png.h>
#include <lua_object.h>

#include <moon_png.h>
#include <mesh_3d.h>
#include <ui_png.h>

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

    Shader text_shader(
        reinterpret_cast<const char*>(text_vert_shader), 
        reinterpret_cast<const char*>(text_frag_shader), nullptr);
    text_shader.use();

    Shader shader(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);

    Shader player_shader(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);

    Shader enemy_shader(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);

    Shader ui_shader(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);

    Shader foreground_shader(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);

    Camera *mainCam = new Camera();
    mainCam->aperture = 45.0f * TAU / 360.0f;
    mainCam->ratio = 1280.0f / 720.0f;
    mainCam->cnear = 0.01f;
    mainCam->cfar = 1000.0f;
    mainCam->eye = glm::vec3{0.0f, 0.0f, 6.0f};
    mainCam->center = glm::vec3{0.0f, 0.0f, 0.0f};
    mainCam->up = glm::vec3{0.0f, 1.0f, 0.0f};

    Player *player = new Player();
    player->camera = mainCam;
    player->shader = &player_shader;
    player->Start();

    ProjectileScene *bulletScene = new ProjectileScene();
    player->currScene = bulletScene;

    Texture *sak_tex = new Texture(sakura1_png, sakura1_png_size);
    Material *sak_mat = new Material();
    sak_mat->SetProperties(&foreground_shader, sak_tex, mainCam, {1, 1, 1, 1});

    GameObject *sakura = new GameObject(sak_mat, {6.0f, -1.0f, 1.0f}, {5.0f, 5.0f, 1.0f});
    GameObject *sakura2 = new GameObject(sak_mat, {8.0f, -1.6f, 2.5f}, {5.0f, 5.0f, 1.0f});

    AnimatedSprite *bg = new AnimatedSprite(1, 1, 0.1f, background_png, background_png_size);
    Transform *bgt = new Transform();
    bgt->position = glm::vec3{0.0f, 0.0f, -2.0f};
    bgt->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    bgt->angle = 0.0f;
    bgt->scaleAmount = glm::vec3{12.0f, 12.0f, 1.0f};
    GameEntity *background = new GameEntity(bg, mainCam);
    background->transform = bgt;
    background->Start();

    Texture *bullet_red = new Texture(bulet2_png, bulet2_png_size);
    Material *red_bullets = new Material;
    red_bullets->SetProperties(&shader, bullet_red, mainCam, {1, 1, 1, 1});

    Texture *bullet_blue = new Texture(bullet1_png, bullet1_png_size);
    Material *blue_bullets = new Material;
    blue_bullets->SetProperties(&shader, bullet_blue, mainCam, {1, 1, 1, 1});

    Texture *ui = new Texture(ui_png, ui_png_size);
    Material *ui_mat = new Material;
    ui_mat->SetProperties(&ui_shader, ui, mainCam, {1, 1, 1, 1});
    GameObject *ui_obj = new GameObject(ui_mat, {0, 0.218f, 5.328f});

    Enemy *enemy = new Enemy();
    enemy->camera = mainCam;
    enemy->shader = &enemy_shader;
    enemy->Start();
    enemy->player = player;
    enemy->currScene = bulletScene;
    enemy->bullet_material = red_bullets;

    for (int i = 0; i < 800; i++)
    {
        ProjectileObject *np = (ProjectileObject *)bulletScene->memory_pool.New();
        np->Initialize();
        bulletScene->inactive_entities.push_back(np);
    }

    TextRenderer *text1 = new TextRenderer("../resources/VT323-Regular.ttf", 26);

    Texture *texture = new Texture(bulet2_png, bulet2_png_size);
    Material *spriteDefault = new Material();
    spriteDefault->SetProperties(&shader, texture, mainCam, {1, 1, 1, 1});

    window->Fire1 = false;
    window->Fire2 = false;
    double delta_time;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_DEBUG_OUTPUT);

    while (window->Running)
    {
        uint64_t startTime = SDL_GetPerformanceCounter();
        SDL_GL_SetSwapInterval(1);
        glClearColor(106.0F / 255.0F, 102.0F / 255.0F, 125.0F / 255.0F, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("projMtx", mainCam->projMatrix());
        shader.setVec4("color", {1, 1, 1, 1});

        background->Update();
        background->Draw();
        window->run();    
            
        if (window->Fire1)
        {
            ProjectileObject *bullet;
            if (bulletScene->inactive_entities.size() > 0)
                bullet = bulletScene->GetFromInactive();
            bullet->Load(blue_bullets, mainCam, glm::vec3{12, 0, 0}, glm::vec3{0, 0, 0},  glm::vec3{player->transform->position.x + 0.4f, player->transform->position.y, 0}, true);
            bulletScene->Add(bullet);
        }

        bulletScene->Update();
        red_bullets->SetupMatrices();
        blue_bullets->SetupMatrices();

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

        float foreground_alpha = 1;
        if(player->transform->position.y < 0 && player->transform->position.y > -1.4f)
        {
            foreground_alpha = std::max(1- (player->transform->position.y / -1.4f), 0.25f);
        }
        if(player->transform->position.y <= -1.4f)
        {
            foreground_alpha = 0.25f;
        }

        foreground_shader.use();
        foreground_shader.setMat4("projMtx", mainCam->projMatrix());
        foreground_shader.setVec4("color", {1, 1, 1, foreground_alpha});

        sakura->transform->position.x -= 2.5f * GameTime::delta_time;
        if(sakura->transform->position.x <= -6.0f)
        {
            sakura->transform->position.x = 6.0f;
        }

        sakura2->transform->position.x -= 2.5f * GameTime::delta_time;
        if (sakura2->transform->position.x <= -6.0f)
        {
            sakura2->transform->position.x = 8.0f;
        }

        enemy->material->SetupMatrices();
        enemy->material->Draw();

        player->material->SetupMatrices();
        player->material->Draw();
        // player->Draw();
        player->DrawLife();
        blue_bullets->Draw();
        red_bullets->Draw();

        sakura->Update();
        sakura2->Update();
        sak_mat->SetupMatrices();
        sak_mat->Draw();

        ui_shader.use();
        ui_shader.setMat4("projMtx", mainCam->projMatrix());
        ui_shader.setVec4("color", {1, 1, 1, 1});
        ui_obj->Update();
        ui_mat->SetupMatrices();
        ui_mat->Draw();

        window->Fire1 = false;
        window->Fire2 = false;
        window->xOff = 0.0f;
        window->zOff = 0.0f;
        std::string fpss = std::to_string((int)(1.0 / GameTime::delta_time)) + " FPS";
        // std::string fpss = std::to_string(player->transform->position.y);
        text1->RenderText(text_shader, fpss.c_str(), 25.0f, 310.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));

        GameTime::draw_calls = 0;
        SDL_GL_SwapWindow(window->Window);
        uint64_t currTime = SDL_GetPerformanceCounter();
        GameTime::delta_time = static_cast<double>(
            (currTime - startTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
    }

    delete spriteDefault;
    delete player;
    delete enemy;
    delete bg;
    delete background;
    delete sakura;
    // delete sakura_entity;
    delete sakura2;
    // delete sakura_entity2;
    delete window;
    delete mainCam;
    delete text1;
    delete texture;
    delete bulletScene;
    delete bullet_red;
    delete red_bullets;
    delete bullet_blue;
    delete blue_bullets;

    return 0;
}