#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

float angle = 0.0;
const int triangle = 1;

void drawCube(float size)
{
  float size2 = size/2;

  glBegin(GL_QUADS);
  // Front
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, -size2, size2);
  glVertex3f(size2, -size2, size2);
  // Back
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(size2, size2, -size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(-size2, -size2, -size2);
  glVertex3f(size2, -size2, -size2);
  // Left
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(-size2, -size2, -size2);
  glVertex3f(-size2, -size2, size2);
  // Right
  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(size2, size2, -size2);
  glVertex3f(size2, -size2, -size2);
  glVertex3f(size2, -size2, size2);
  // Top
  glColor3f(1.0, 0.0, 1.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(size2, size2, -size2);
  // Bottom
  glColor3f(0.0, 1.0, 1.0);
  glVertex3f(size2, -size2, size2);
  glVertex3f(-size2, -size2, size2);
  glVertex3f(-size2, -size2, -size2);
  glVertex3f(size2, -size2, -size2);

  glEnd();

  return;
}

void init(SDL_Surface* screen)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<float>(screen->w)/static_cast<float>(screen->h), 1.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH); // This is the default setting anyways.
  glEnable(GL_DEPTH_TEST);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
  glRotatef(angle, 1.0, 1.0, 1.0);
  drawCube(1.0);
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
