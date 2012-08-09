#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

float angle = 0.0;
const int triangle = 1;

unsigned int loadTexture(const char* filename)
{
  SDL_Surface* image = SDL_LoadBMP(filename);

  unsigned int id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, image->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  SDL_FreeSurface(image);

  return id;
}

unsigned int tex;

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

  tex = loadTexture("img/texture/crackedwall.bmp");
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  glTranslatef(0.0, 0.0, -5.0);
  glRotatef(angle, 1.0, 1.0, 1.0);

  glBindTexture(GL_TEXTURE_2D, tex);

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 2.0);
  glVertex3f(-2.0, 2.0, 0.0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-2.0, -2.0, 0.0);
  glTexCoord2f(2.0, 0.0);
  glVertex3f(2.0, -2.0, 0.0);
  glTexCoord2f(2.0, 2.0);
  glVertex3f(2.0, 2.0, 0.0);
  glEnd();
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
