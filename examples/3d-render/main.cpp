#include <window.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>

#include <material.h>
#include <mesh_3d.h>
#include <tile.h>
#include <texture.h>
#include <game_object.h>
#include <sprite_single_vertex_shader.h>
#include <sprite_single_frag_shader.h>
#include <text_renderer.h>
#include <sakfly_png.h>
#include <baked_png.h>

#define WINDOW_TITLE "Mahou Engine - 3D Render"
#define WIDTH 640
#define HEIGHT 360
constexpr auto TAU = glm::two_pi<float>();

int main(int ArgCount, char **Args)
{

    Window *window = new Window;
    window->init(WINDOW_TITLE, WIDTH, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);

    Camera *mainCam = new Camera();
    mainCam->aperture = 45.0f * TAU / 360.0f;
    mainCam->ratio = 1280.0f / 720.0f;
    mainCam->cnear = 0.01f;
    mainCam->cfar = 1000.0f;
    mainCam->eye = glm::vec3{0.0f, 0.0f, 6.0f};
    mainCam->center = glm::vec3{0.0f, 0.0f, 0.0f};
    mainCam->up = glm::vec3{0.0f, 1.0f, 0.0f};
    Shader shader(
        reinterpret_cast<const char *>(sprite_single_vertex_shader),
        reinterpret_cast<const char *>(sprite_single_frag_shader), nullptr);
    Texture *texture = new Texture(baked_png, baked_png_size);

    Mesh3D mesh("../resources/model.obj", mainCam, &shader, texture);
    mesh.transform.position = glm::vec3{3.782f, -1.47f, 2.939f};
    mesh.transform.rotationAxis = glm::vec3{0, 1, 0};

    Mesh3D mesh2("../resources/model.obj", mainCam, &shader, texture);
    mesh2.transform.position = glm::vec3{3.782f + 4.5f, -1.87f, 2.3f};
    mesh2.transform.rotationAxis = glm::vec3{0, 1, 0};
    mesh.transform.angle = 90.0f;

    Shader text_shader(
        reinterpret_cast<const char *>(text_vert_shader),
        reinterpret_cast<const char *>(text_frag_shader), nullptr);
    TextRenderer text1("../resources/VT323-Regular.ttf", 26);

    Shader instanced(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);

    Texture *sak_tex = new Texture(sakfly_png, sakfly_png_size);
    Material *spriteDefault = new Material();
    spriteDefault->SetProperties(&instanced, sak_tex, mainCam, {1, 1, 1, 1});
    GameObject *player = new GameObject();
    player->camera = mainCam;
    player->SetupPlane(0, 0, 1, 1);
    player->MoveTo(0, 0, 4);

    player->AssignMaterial(spriteDefault);
    player->AssignSprite(4, 1);
    player->sprite->AddAnimation(std::vector<int>{0, 1, 2, 3});
    player->sprite->SwitchAnimation(1, 0.08f);
    const float player_speed = 0.0005f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

        instanced.use();
        instanced.setMat4("projMtx", mainCam->projMatrix());
        instanced.setVec4("color", {1, 1, 1, 1});

        window->run();
        if(mesh.transform.position.x <= -3.82f) mesh.transform.position = glm::vec3{3.782f, -1.47f, 2.939f};
        if(mesh2.transform.position.x <= -4.82f) mesh2.transform.position = glm::vec3{3.782f + 4.5f, -1.87f, 2.3f};

        mesh.transform.position -= glm::vec3{0.00035f, 0, 0};
        mesh2.transform.position -= glm::vec3{0.00035f, 0, 0};

        mesh.Update();
        mesh.Draw();

        mesh2.Update();
        mesh2.Draw();

        player->transform->position += glm::vec3{-window->xOff * player_speed, window->zOff * player_speed, 0};
        player->transform->position = glm::vec3{(std::clamp(player->transform->position.x, -1.19f, 1.22f)),
                                                (std::clamp(player->transform->position.y, -0.67f, 0.57f)),
                                                player->transform->position.z};
        player->Update();
        spriteDefault->SetupMatrices();
        spriteDefault->Draw();

        std::string tree_pos = "x: " + std::to_string(player->transform->position.x) + ", y: " + std::to_string(player->transform->position.y) + ", z: " + std::to_string(player->transform->position.z);
        text1.RenderText(text_shader, tree_pos.c_str(), 25.0f, 310.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));

        SDL_GL_SwapWindow(window->Window);
        uint64_t currTime = SDL_GetPerformanceCounter();
        GameTime::delta_time = static_cast<double>(
            (currTime - startTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
    }
    delete spriteDefault;
    delete player;
    delete texture;
    delete mainCam;
    delete window;
    return 0;
}