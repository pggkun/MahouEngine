#include <window.h>
#include <stdio.h>
#include <time.h>

#include <material.h>
#include <mesh.h>
#include <tile.h>
#include <texture.h>
#include <game_object.h>

#include <mahou_logo_png.h>

#define WINDOW_TITLE "Mahou Engine"
#define WIDTH 640
#define HEIGHT 360
constexpr auto TAU = glm::two_pi<float>();

int main(int ArgCount, char **Args)
{
    Window *window = new Window;
    window->init(WINDOW_TITLE, WIDTH, HEIGHT);
    // glewInit();
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
        reinterpret_cast<const char *>(sprite_inst_vert_shader),
        reinterpret_cast<const char *>(sprite_inst_frag_shader), nullptr);
    Texture *texture = new Texture(mahou_logo_png, mahou_logo_png_size);
    Material *spriteDefault = new Material();
    spriteDefault->SetProperties(&shader, texture, mainCam, {1, 1, 1, 1});
    GameObject *logo = new GameObject();
    logo->transform->scaleAmount = glm::vec3(6.5,6.5,1);
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

    glEnable(GL_DEBUG_OUTPUT);
    while (window->Running)
    {
        uint64_t startTime = SDL_GetPerformanceCounter();
        SDL_GL_SetSwapInterval(0);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.setMat4("projMtx", mainCam->projMatrix());

        if(counter < fadeDuration)
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
    delete texture;
    delete logo;
    delete spriteDefault;
    delete mainCam;
    delete window;
    return 0;
}