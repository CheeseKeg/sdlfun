#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "extra.h"
#include "Camera.h"

bool mousein = false;
const int triangle = 1;

unsigned int tex;

SDL_Surface* screen;
Camera* cam;

void init(SDL_Surface* screen)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<float>(screen->w)/static_cast<float>(screen->h), 1.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH); // This is the default setting anyways.

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  tex = loadTexture("img/texture/crackedwall.bmp", false, true);

  cam = new Camera(0.0, 0.0, 0.0, 0.0, 0.0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  cam->Control(0.2, 0.2, mousein, screen->w, screen->h);
  cam->Update();

  glTranslatef(0.0, 0.0, -5.0);

  glBindTexture(GL_TEXTURE_2D, tex);

  float s = 100.0;
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, s);
  glVertex3f(-s, -2.0, s);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-s, -2.0, -s);
  glTexCoord2f(s, 0.0);
  glVertex3f(s, -2.0, -s);
  glTexCoord2f(s, s);
  glVertex3f(s, -2.0, s);
  glEnd();
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
  return 0;
}
