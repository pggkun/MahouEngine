#include <window.h>
#include <stdio.h>
#include <time.h>

#include <material.h>
#include <mesh_3d.h>
#include <tile.h>
#include <texture.h>
#include <game_object.h>
#include <sprite_single_vertex_shader.h>
#include <sprite_single_frag_shader.h>

#include <texture_png.h>

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
    Texture *texture = new Texture(texture_png, texture_png_size);

    Mesh3D mesh("../resources/model.obj", mainCam, &shader, texture);
    mesh.transform.position = glm::vec3{0, 0, 4};
    mesh.transform.rotationAxis = glm::vec3{0, 1, 0};

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

        window->run();
        mesh.transform.position += glm::vec3{window->xOff * 0.0001f, 0, window->zOff * 0.0001f};
        mesh.transform.angle += 0.0001f;
        mesh.Update();
        mesh.Draw();   

        SDL_GL_SwapWindow(window->Window);
        uint64_t currTime = SDL_GetPerformanceCounter();
        GameTime::delta_time = static_cast<double>(
            (currTime - startTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
    }
    delete texture;
    delete mainCam;
    delete window;
    return 0;
}