#include <SDL/SDL.h>

int main (int argc, char** argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface* screen;
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  bool running = true;
  const int FPS = 30;
  Uint32 start;
  bool b[4] = {false, false, false, false};

  SDL_Rect rect;
  rect.x = rect.y = 10;
  rect.w = rect.h = 20;
  Uint32 color = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  Uint32 color2 = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);

  while (running)
  {
    start = SDL_GetTicks();
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
	running = false;
	break;
      case SDL_KEYDOWN:
	switch (event.key.keysym.sym)
	{
	case SDLK_UP:
	  b[0] = true;
	  break;
	case SDLK_DOWN:
	  b[1] = true;
	  break;
	case SDLK_LEFT:
	  b[2] = true;
	  break;
	case SDLK_RIGHT:
	  b[3] = true;
	  break;
	default:
	  break;
	}
	break;
      case SDL_KEYUP:
	switch (event.key.keysym.sym)
	{
	case SDLK_UP:
	  b[0] = false;
	  break;
	case SDLK_DOWN:
	  b[1] = false;
	  break;
	case SDLK_LEFT:
	  b[2] = false;
	  break;
	case SDLK_RIGHT:
	  b[3] = false;
	  break;
	default:
	  break;
	}
	break;
      default:
	break;
      }
    }
    // Logic & render

    if (b[0])
      rect.y--;
    if (b[1])
      rect.y++;
    if (b[2])
      rect.x--;
    if (b[3])
      rect.x++;

    SDL_FillRect(screen, &screen->clip_rect, color);
    SDL_FillRect(screen, &rect, color2);

    SDL_Flip(screen);

    if (SDL_GetTicks() - start < 1000/FPS)
    {
      SDL_Delay(1000/FPS - (SDL_GetTicks()-start));
    }
  }

  SDL_Quit();
  return 0;
}
