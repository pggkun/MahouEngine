#include <window.h>
#include <stdio.h>
#include <time.h>

#include <material.h>
#include <mesh.h>
#include <tile.h>
#include <texture.h>
#include <game_object.h>

#include <mahou_logo_png.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "projectile_scene.h"
#include "enemy.h"
#include <bullet1_png.h>

#define WINDOW_TITLE "Mahou Engine - Patterns"
#define WIDTH 1280
#define HEIGHT 720
constexpr auto TAU = glm::two_pi<float>();

int main(int ArgCount, char **Args)
{
    ImGui::CreateContext();
    Window *window = new Window;
    window->init(WINDOW_TITLE, WIDTH, HEIGHT);
    // glewInit();
    glViewport(0, 0, WIDTH, HEIGHT);

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window->Window, window->Context);
    const char *glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    Camera *mainCam = new Camera();
    mainCam->aperture = 45.0f * TAU / 360.0f;
    mainCam->ratio = 1280.0f / 720.0f;
    mainCam->cnear = 0.01f;
    mainCam->cfar = 1000.0f;
    mainCam->eye = glm::vec3{0.0f, 0.0f, 6.0f};
    mainCam->center = glm::vec3{0.0f, 0.0f, 0.0f};
    mainCam->up = glm::vec3{0.0f, 1.0f, 0.0f};

    Shader shader(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);
    Texture *texture = new Texture(mahou_logo_png, mahou_logo_png_size);
    Material *spriteDefault = new Material();
    spriteDefault->SetProperties(&shader, texture, mainCam, {1, 1, 1, 1});
    GameObject *logo = new GameObject();
    logo->transform->scaleAmount = glm::vec3(4,4,1);
    logo->camera = mainCam;
    logo->SetupPlane(0, 0, 1, 1);
    logo->transform->position = glm::vec3(3.2f, -2.0f, -1.539f);
    logo->AssignMaterial(spriteDefault);
    logo->AssignSprite(1, 1);

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_ALWAYS);

    float fadeDuration = 1.5f;
    float counter = 0.0f;
    float counterDur = 0.0f;
    float counterOut = 0.0f;

    ProjectileScene *bulletScene = new ProjectileScene();
    Shader bulletShader(
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);

    Texture *bullet_blue = new Texture(bullet1_png, bullet1_png_size);
    Material *blue_bullets = new Material;
    blue_bullets->SetProperties(&shader, bullet_blue, mainCam, {1, 1, 1, 1});

    Enemy *enemy = new Enemy(nullptr, mainCam);
    enemy->Start();
    enemy->currScene = bulletScene;
    enemy->bullet_material = blue_bullets;

    for (int i = 0; i < 1000; i++)
    {
        ProjectileObject *np = (ProjectileObject *)bulletScene->memory_pool.New();
        np->Initialize();
        bulletScene->inactive_entities.push_back(np);
    }

    bool show_demo_window = true;

    static int directions = 50;
    static float interval = enemy->shoot_interval;
    static float bullet_speed = enemy->bullet_speed;
    static float acceleration = enemy->dir_acc;

    glEnable(GL_DEBUG_OUTPUT);
    while (window->Running)
    {
        // window->run();
        while (SDL_PollEvent(&window->Event))
        {
            ImGui_ImplSDL2_ProcessEvent(&window->Event);
            if (window->Event.type == SDL_QUIT)
                window->Running = false;
            if (window->Event.type == SDL_WINDOWEVENT && window->Event.window.event == SDL_WINDOWEVENT_CLOSE && window->Event.window.windowID == SDL_GetWindowID(window->Window))
                window->Running = false;
        }

        
        
        float cpos[3] = {
            enemy->transform->position.x,
            enemy->transform->position.y,
            enemy->transform->position.z};

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Circular Pattern", &show_demo_window, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("%d FPS", (int)(1.0 / GameTime::delta_time));
        ImGui::Text("Active Bullets: %d", bulletScene->entities.size());
        ImGui::Text("Inactive Bullets: %d", bulletScene->inactive_entities.size());
        ImGui::SliderFloat("Interval", &interval, 0.3f, 5.0f);
        ImGui::SliderFloat("Speed", &bullet_speed, 0.3f, 5.0f);
        ImGui::SliderFloat("Acceleration", &acceleration, -5.0f, 5.0f);
        ImGui::SliderInt("Directions", &directions, 1, 360); 
        ImGui::SameLine();
        if(ImGui::Button("Update"))
            enemy->GetDirections(directions);
        ImGui::SliderFloat3("Position", cpos, -4.5, 4.5);
        // ImGui::Text("Apenas um texto de teste");
        ImGui::End();
        ImGui::Render();

        enemy->transform->position = glm::vec3(cpos[0], cpos[1], cpos[2]);
        enemy->shoot_interval = interval;
        enemy->bullet_speed = bullet_speed;
        enemy->dir_acc = acceleration;

        uint64_t startTime = SDL_GetPerformanceCounter();
        // SDL_GL_SetSwapInterval(0);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.setMat4("projMtx", mainCam->projMatrix());
        shader.setVec4("color", {1, 1, 1, 1});
        /*if(counter < fadeDuration)
        {
            shader.setVec4("color", {1, 1, 1, (counter / fadeDuration)});
            counter += GameTime::delta_time;
        }
        else if (counterDur < fadeDuration)
        {
            shader.setVec4("color", {1, 1, 1, 1});
            counterDur += GameTime::delta_time;
        }
        else if (counterOut < fadeDuration)
        {
            shader.setVec4("color", {1, 1, 1, 1 - (counterOut / fadeDuration)});
            counterOut += GameTime::delta_time;
        } */
        spriteDefault->SetupMatrices();
        spriteDefault->Draw();

        bulletShader.use();
        bulletShader.setMat4("projMtx", mainCam->projMatrix());
        bulletShader.setVec4("color", {1, 1, 1, 1});

        bulletScene->Update();
        blue_bullets->SetupMatrices();
        blue_bullets->Draw();

        

        logo->Update();
        enemy->Update();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window->Window);
        uint64_t currTime = SDL_GetPerformanceCounter();
        GameTime::delta_time = static_cast<double>(
            (currTime - startTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
    }
    delete texture;
    delete logo;
    delete spriteDefault;
    delete mainCam;
    delete window;
    return 0;
}