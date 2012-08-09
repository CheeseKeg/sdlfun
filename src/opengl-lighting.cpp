#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

float angle = 0.0;
const int triangle = 1;

void drawCube(float size)
{
  float size2 = size/2;
  float diffuseambient[] = {1.0, 0.5, 0.3, 1.0};

  glBegin(GL_QUADS);
  // Front
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuseambient);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, -size2, size2);
  glVertex3f(size2, -size2, size2);
  // Back
  glNormal3f(0.0, 0.0, -1.0);
  glVertex3f(size2, size2, -size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(-size2, -size2, -size2);
  glVertex3f(size2, -size2, -size2);
  // Left
  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(-size2, -size2, -size2);
  glVertex3f(-size2, -size2, size2);
  // Right
  glNormal3f(1.0, 0.0, 0.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(size2, size2, -size2);
  glVertex3f(size2, -size2, -size2);
  glVertex3f(size2, -size2, size2);
  // Top
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(size2, size2, -size2);
  // Bottom
  glNormal3f(0.0, -1.0, 0.0);
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
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //glEnable(GL_COLOR_MATERIAL);

  float diffuse[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  float ambience[] = {0.2, 0.2, 0.2, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambience);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  float position[] = {-2.0, 2.0, -3.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, position);

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
