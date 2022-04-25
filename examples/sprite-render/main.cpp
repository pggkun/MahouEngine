#include <window.h>
#include <stdio.h>
#include <time.h>
#include <gl/glcorearb.h>
#include <gl/glext.h>

#include <material.h>
#include <mesh.h>
#include <texture.h>
#include <game_object.h>

#include <bichin_png.h>

#define WINDOW_TITLE "Mahou Engine - Sprite Render"
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

    Shader shader(vs, fs, nullptr);
    Texture *texture = new Texture(bichin_png, bichin_png_size);
    Material *spriteDefault = new Material();
    spriteDefault->SetProperties(&shader, texture, mainCam, {1, 1, 1, 1});
    GameObject *simplePlane = new GameObject();
    simplePlane->camera = mainCam;
    simplePlane->SetupPlane(0, 0, 1, 1);
    simplePlane->MoveTo(0, 0);
    simplePlane->AssignMaterial(spriteDefault);
    simplePlane->AssignSprite(4, 1);
    simplePlane->sprite->AddAnimation(std::vector<int>{0, 1, 2, 3});
    simplePlane->sprite->SwitchAnimation(1, 0.08f);

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_ALWAYS);

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

        window->run();

        simplePlane->Update();
        spriteDefault->SetupMatrices();
        spriteDefault->Bind();
        spriteDefault->Draw();

        SDL_GL_SwapWindow(window->Window);
        uint64_t currTime = SDL_GetPerformanceCounter();
        GameTime::delta_time = static_cast<double>(
            (currTime - startTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
    }
    delete texture;
    delete simplePlane;
    delete spriteDefault;
    delete mainCam;
    delete window;
    return 0;
}