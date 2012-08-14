#include <SDL/SDL.h>
#include <GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "Camera.h"
#include <fstream>

bool mousein = false;

Camera* cam;

unsigned int vert, frag, prog;

void loadFile(const char* filename, std::string& str)
{
  std::ifstream in(filename);
  if (!in.is_open())
  {
    std::cout << "Unable to load file: " << filename << std::endl;
    return;
  }

  char line[300];
  while (!in.eof())
  {
    in.getline(line, 300);
    str += line;
    str += '\n';
  }

  return;
}

unsigned int loadShader(std::string& source, unsigned int mode)
{
  unsigned int id;

  id = glCreateShader(mode);

  const char* csource = source.c_str();
  glShaderSource(id, 1, &csource, NULL);

  glCompileShader(id);

  char error[1000];
  glGetShaderInfoLog(id, 1000, NULL, error);

  std::cout << "Shader compile status:" << std::endl << error << std::endl;

  return id;
}

void initShader(const char* vname, const char* fname, unsigned int& program, unsigned int& vs, unsigned int& fs)
{
  std::string source;

  loadFile(vname, source);
  vs = loadShader(source, GL_VERTEX_SHADER);

  source.clear();

  loadFile(fname, source);
  fs = loadShader(source, GL_FRAGMENT_SHADER);

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);

  glLinkProgram(program);
  glUseProgram(program);

  return;
}

void clean()
{
  glDetachShader(prog, vert);
  glDetachShader(prog, frag);

  glDeleteShader(vert);
  glDeleteShader(frag);

  glDeleteProgram(prog);

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

  cam = new Camera(0.0, 0.0, 0.0, 0.0, 0.0);

  const unsigned char* version = glGetString(GL_VERSION);
  std::cout << "OpenGL version: " << version << std::endl;

  initShader("shader/vertex.vs", "shader/fragment.frag", prog, vert, frag);

  return;
}

void display(SDL_Surface* screen)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  cam->Control(0.2, 0.2, mousein, screen->w, screen->h);
  cam->Update();

  glBegin(GL_TRIANGLES);
  glVertex3f(0.0, 1.0, -4.0);
  glVertex3f(-1.0, -1.0, -4.0);
  glVertex3f(1.0, -1.0, -4.0);
  glEnd();

  return;
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

    display(screen);
    SDL_GL_SwapBuffers();

    if (SDL_GetTicks() - start < 1000/FPS)
    {
      SDL_Delay(1000/FPS - (SDL_GetTicks()-start));
    }
  }

  clean();
  SDL_Quit();
  return 0;
}
