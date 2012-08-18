#include "objloader.h"
#include "Camera.h"
#include "Skybox.h"

float angle = 0.0;
bool mousein = false;

SDL_Surface* screen;
objloader obj;
Skybox* sky;
Camera* cam;
int cube;

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

  cube = obj.load("model/obj/monkey.obj");
  cam = new Camera(0.0, 0.0, -5.0, 0.0, 0.0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  cam->Control(0.2, 0.2, mousein, screen->w, screen->h);
  sky->Draw();
  cam->Update();

  float lightposition[] = {-1.0, 1.0, -2.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

  glPushMatrix();

  //glTranslatef(0.0, 0.0, -10.0);
  glRotatef(angle, 1.0, 1.0, 1.0);
  glCallList(cube);

  glPopMatrix();
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
  sky->Kill();

  return 0;
}
