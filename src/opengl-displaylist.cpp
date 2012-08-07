#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

float angle = 0.0;
const int triangle = 1;

void init(SDL_Surface* screen)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<float>(screen->w)/static_cast<float>(screen->h), 1.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH); // This is the default setting anyways.

  glNewList(triangle, GL_COMPILE); // TRIANGLE LIST

  glBegin(GL_TRIANGLES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.0, 2.0, 0.0);
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(-2.0, -2.0, 0.0);
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(2.0, -2.0, 0.0);
  glEnd();

  glEndList(); // END TRIANGLE LIST
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
  glRotatef(angle, 0.0, 1.0, 0.0);
  glScalef(1.0, 0.5, 1.0);

  glCallList(triangle);
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

    if (SDL_GetTicks() - start < 1000/FPS)
    {
      SDL_Delay(1000/FPS - (SDL_GetTicks()-start));
    }
  }

  SDL_Quit();
  return 0;
}
