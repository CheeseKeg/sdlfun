#include "objloader.h"

float angle = 0.0;

int cube;
objloader obj;

void init(SDL_Surface* screen)
{
  glClearColor(0.5, 0.5, 0.5, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<float>(screen->w)/static_cast<float>(screen->h), 1.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH); // This is the default setting anyways.

  glEnable(GL_DEPTH_TEST);

  cube = obj.load("model/obj/uvcube.obj");

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  float lightcolor[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // (A*S) + (B*D)

  //cube = loadObject("model/obj/monkeymed.obj");
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  float lightposition[] = {-1.0, 1.0, -2.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

  glTranslatef(0.0, 0.0, -10.0);
  glRotatef(angle, 1.0, 1.0, 1.0);
  glCallList(cube);
}

int main (int argc, char** argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Surface* screen;
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
      switch (event.type)
      {
      case SDL_QUIT:
	running = false;
	break;
      default:
	break;
      }
    }

    display();
    SDL_GL_SwapBuffers();

    angle += 0.5;
    if (angle >= 360)
    {
      angle -= 360;
    }

    if (SDL_GetTicks() - start < 1000/FPS)
    {
      SDL_Delay(1000/FPS - (SDL_GetTicks()-start));
    }
  }

  SDL_Quit();
  return 0;
}
