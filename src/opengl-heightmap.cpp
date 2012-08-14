#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Camera.h"
#include "Skybox.h"
#include <vector>

bool mousein = false;

SDL_Surface* screen;
Skybox* sky;
Camera* cam;

std::vector< std::vector<float> > heights;

void init(SDL_Surface* screen)
{
  glClearColor(0.5, 0.5, 0.5, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<float>(screen->w)/static_cast<float>(screen->h), 1.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH); // This is the default setting anyways.

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  float lightcolor[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // (A*S) + (B*D)

  sky = new Skybox();
  sky->Init("img/skybox/");
  sky->Compile(50.0);

  cam = new Camera(0.0, 0.0, -5.0, 0.0, 0.0);

  loadHeightmap("img/heightmap/heightmap.bmp", heights);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  cam->Control(0.2, 0.2, mousein, screen->w, screen->h);

  glColor3f(1.0, 1.0, 1.0);
  sky->Draw();

  cam->Update();

  float lightposition[] = {-1.0, 1.0, -2.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

  //glDisable(GL_LIGHTING);
  renderHeightmap(0.1, 5, heights);
  //glEnable(GL_LIGHTING);
}

int main (int argc, char** argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | SDL_OPENGL);
  init(screen);

  bool running = true;
  Uint32 start;

  unsigned const int FPS = 30;

  SDL_Event event;
  while (running)
  {
    start = SDL_GetTicks();

    while (SDL_PollEvent(&event))
    {
      SDLKey* k = &event.key.keysym.sym;

      switch (event.type)
      {
      case SDL_QUIT:
	running = false;
	break;
      case SDL_MOUSEBUTTONDOWN:
	mousein = true;
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouse(screen->w/2, screen->h/2);
      case SDL_KEYDOWN:
	if (*k == SDLK_p)
	{
	  mousein = false;
	  SDL_ShowCursor(SDL_ENABLE);
	}
	else if (*k == SDLK_ESCAPE)
	{
	  running = false;
	}

	break;
      default:
	break;
      }
    }

    display();
    SDL_GL_SwapBuffers();

    if (SDL_GetTicks() - start < 1000/FPS)
    {
      SDL_Delay(1000/FPS - (SDL_GetTicks()-start));
    }
  }

  SDL_Quit();
  sky->Kill();

  return 0;
}
