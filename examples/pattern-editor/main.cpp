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
#include <kunai_blue_png.h>
#include <kibe_blue_png.h>

#define WINDOW_TITLE "Mahou Engine - Pattern Editor"
#define WIDTH 1280
#define HEIGHT 720
constexpr auto TAU = glm::two_pi<float>();

void ScreenToWorld(int mousex, int mousey, float depth, Camera *cam, glm::vec3 *res)
{
    const float half_screen_width = WIDTH / 2.0f;
    const float half_screen_height = HEIGHT / 2.0f;

    glm::mat4 m_proj;
    glm::mat4 depthProjection = glm::perspective(cam->aperture, cam->ratio, cam->cnear, abs(cam->eye.z - depth));

    for (glm::mat<4, 4, float, glm::packed_highp>::length_type i = 0; i < 4; i++)
        for (glm::mat<4, 4, float, glm::packed_highp>::length_type j = 0; j < 4; j++)
            m_proj[i][j] = depthProjection[i][j];

    glm::mat4 m_view;

    for (glm::mat<4, 4, float, glm::packed_highp>::length_type i = 0; i < 4; i++)
        for (glm::mat<4, 4, float, glm::packed_highp>::length_type j = 0; j < 4; j++)
            m_view[i][j] = cam->viewMatrix()[i][j];

    glm::mat4 inv_mat = glm::inverse(m_proj * m_view);

    glm::vec4 n((mousex - half_screen_width) / half_screen_width, -1 * (mousey - half_screen_height) / half_screen_height, -1, 1.0);
    glm::vec4 f((mousex - half_screen_width) / half_screen_width, -1 * (mousey - half_screen_height) / half_screen_height, 1, 1.0);

    glm::vec4 near_result = inv_mat * n;
    glm::vec4 far_result = inv_mat * f;

    near_result /= near_result.w;
    far_result /= far_result.w;

    glm::vec3 dir = (glm::vec3(far_result - near_result));
    res->x = dir.x;
    res->y = dir.y;
    res->z = dir.z;
}

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
    
    logo->MoveTo(0, 0);
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
    bullet_blue->GenerateTexture(GL_NEAREST);
    Texture *kibe_blue = new Texture(kibe_blue_png, kibe_blue_png_size);
    kibe_blue->GenerateTexture(GL_NEAREST);
    Texture *kunai_blue = new Texture(kunai_blue_png, kunai_blue_png_size);
    kunai_blue->GenerateTexture(GL_NEAREST);

    Material *blue_bullets = new Material;
    blue_bullets->SetProperties(&shader, bullet_blue, mainCam, {1, 1, 1, 1});

    Enemy *enemy = new Enemy(nullptr, mainCam);
    enemy->Start();
    enemy->currScene = bulletScene;
    enemy->bullet_material = blue_bullets;

    for (int i = 0; i < 2000; i++)
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
    static float bullet_scale = bulletScene->scale;
    static float enemy_angle = enemy->transform->angle;
    static int xMouse, yMouse;


    glEnable(GL_DEBUG_OUTPUT);

    while (counterOut < fadeDuration)
    {
        uint64_t startTime = SDL_GetPerformanceCounter();
        // SDL_GL_SetSwapInterval(0);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.setMat4("projMtx", mainCam->projMatrix());

        if (counter < fadeDuration)
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
        }

        window->run();

        spriteDefault->SetupMatrices();
        spriteDefault->Draw();

        logo->Update();

        SDL_GL_SwapWindow(window->Window);
        uint64_t currTime = SDL_GetPerformanceCounter();
        GameTime::delta_time = static_cast<double>(
            (currTime - startTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
    }
    logo->transform->position = glm::vec3(3.2f, -2.0f, -1.539f);
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
            if (window->Event.type == SDL_MOUSEMOTION)
            {
                if (SDL_GetMouseState(nullptr, nullptr)&SDL_BUTTON_RMASK)
                    SDL_GetGlobalMouseState(&xMouse, &yMouse);
            }
                
        }

        float cpos[3] = {
            enemy->transform->position.x,
            enemy->transform->position.y,
            enemy->transform->position.z};

        glm::vec3 worldPos;
        ScreenToWorld(xMouse, yMouse, 0, mainCam, &worldPos);
        glm::vec3 offset = glm::vec3(2.07445f-1.226748f,(-0.509996)-(-0.082702), 0);
        enemy->transform->position = glm::vec3(worldPos.x, worldPos.y, enemy->transform->position.z) - offset;            
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Circle Pattern", &show_demo_window, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("%d FPS", (int)(1.0 / GameTime::delta_time));
        ImGui::Text("Active Bullets: %d", bulletScene->entities.size());
        ImGui::Text("Inactive Bullets: %d", bulletScene->inactive_entities.size());
        ImGui::SliderFloat("Interval", &interval, 0.01f, 5.0f);
        ImGui::SliderFloat("Speed", &bullet_speed, 0.01f, 5.0f);
        ImGui::SliderFloat("Acceleration", &acceleration, -5.0f, 5.0f);
        ImGui::SliderFloat("Scale", &bullet_scale, 0.01f, 1.0f);
        // ImGui::SliderFloat("Angle", &enemy_angle, 0.0f, 360.0f);
        ImGui::InputInt("Directions", &directions); 
        ImGui::SameLine();
        if(ImGui::Button("Update"))
            enemy->GetDirections(directions);
        ImGui::SliderFloat3("Position", cpos, -4.5, 4.5);
        ImGui::Text("mouse (x: %d, y: %d)", xMouse, yMouse);
        ImGui::Text("world (x: %f, y: %f, z: %f)", worldPos.x, worldPos.y, worldPos.z);
        if(ImGui::Button("Clear"))
        {
            list<ProjectileObject *>::iterator prox = bulletScene->entities.begin();
            while (prox != bulletScene->entities.end())
            {
                list<ProjectileObject *>::iterator it = prox++;
                (*it)->RemoveMaterial();
                bulletScene->inactive_entities.push_back(*it);
                bulletScene->entities.erase(it);
            }
        }
        if(ImGui::ImageButton((void *)(intptr_t)kunai_blue->textureID, ImVec2(50,50)))
            blue_bullets->SetProperties(&shader, kunai_blue, mainCam, {1, 1, 1, 1});
        ImGui::SameLine();
        if (ImGui::ImageButton((void *)(intptr_t)kibe_blue->textureID, ImVec2(50, 50)))
            blue_bullets->SetProperties(&shader, kibe_blue, mainCam, {1, 1, 1, 1});
        ImGui::SameLine();
        if (ImGui::ImageButton((void *)(intptr_t)bullet_blue->textureID, ImVec2(50, 50)))
            blue_bullets->SetProperties(&shader, bullet_blue, mainCam, {1, 1, 1, 1});

        ImGui::End();
        ImGui::Render();

        enemy->shoot_interval = interval;
        enemy->bullet_speed = bullet_speed;
        enemy->dir_acc = acceleration;
        enemy->transform->angle = enemy_angle;
        bulletScene->scale = bullet_scale;

        uint64_t startTime = SDL_GetPerformanceCounter();
        // SDL_GL_SetSwapInterval(0);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        logo->Update();
        shader.use();
        shader.setMat4("projMtx", mainCam->projMatrix());
        shader.setVec4("color", {1, 1, 1, 1});

        spriteDefault->SetupMatrices();
        spriteDefault->Draw();

        bulletShader.use();
        bulletShader.setMat4("projMtx", mainCam->projMatrix());
        bulletShader.setVec4("color", {1, 1, 1, 1});

        bulletScene->Update();
        blue_bullets->SetupMatrices();
        blue_bullets->Draw();

        
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