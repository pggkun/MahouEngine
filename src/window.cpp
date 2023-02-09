#include "window.h"
#include <stdio.h>

void Window::init(const char *title, u32 Width, u32 Heigth)
{
    this->WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    this->Window = SDL_CreateWindow(title, 0, 0, Width, Heigth, WindowFlags);
    SDL_SetWindowPosition(this->Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    assert(this->Window);
    this->Context = SDL_GL_CreateContext(this->Window);
    this->renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);

    this->Running = 1;
    this->FullScreen = 0;
}

void Window::run()
{
  float step = 0.01f;
  while (SDL_PollEvent(&this->Event))
  {
    if (Event.type == SDL_MOUSEMOTION)
    {
      SDL_GetGlobalMouseState(&this->mouseX, &this->mouseY);
    }
    if (Event.type == SDL_MOUSEBUTTONDOWN)
    {
      if (Event.button.button == SDL_BUTTON_LEFT)
      {
        Fire1 = true;
      }
      else if(Event.button.button == SDL_BUTTON_RIGHT)
      {
        Fire2 = true;
      }
    }
    
    if (Event.type == SDL_KEYDOWN)
    {
      switch (Event.key.keysym.sym)
      {
        case SDLK_ESCAPE:
          Running = 0;
          break;
        default:
          break;
      }
    }
    else if (Event.type == SDL_QUIT)
    {
      Running = 0;
    }
  }
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  
  if (keystates[SDL_SCANCODE_LSHIFT])
  {
    this->Shift = true;
  }
  else
  {
    this->Shift = false;
  }
  if (keystates[SDL_SCANCODE_A])
  {
    this->xOff =1.0f;
  }
  else if (keystates[SDL_SCANCODE_D])
  {
    this->xOff =-1.0f;
  }
  else{
    this->xOff = 0.0f;
  }
  
  if (keystates[SDL_SCANCODE_W])
  {
    this->zOff = 1.0f;
  }
  else if (keystates[SDL_SCANCODE_S])
  {
    this->zOff = -1.0f;
  }
  else
  {
    this->zOff = 0.0f;
  }
  if (keystates[SDL_SCANCODE_Q])
  {
    yOff = 1;
  }
  else if (keystates[SDL_SCANCODE_E])
  {
    yOff = -1;
  }
  else
  {
      yOff = 0.0f;
  }
  if (keystates[SDL_SCANCODE_RIGHT])
  {
    this->horizontal = -1.0f;
  }
  else if (keystates[SDL_SCANCODE_LEFT])
  {
    this->horizontal = 1.0f;
  }
  else{
    this->horizontal = 0.0f;
  }

  if (keystates[SDL_SCANCODE_UP])
  {
    this->vertical = 1.0f;
  }
  else if (keystates[SDL_SCANCODE_DOWN])
  {
    this->vertical = -1.0f;
  }
  else
  {
    this->vertical = 0.0f;
  }
}
