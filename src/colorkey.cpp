#include <SDL/SDL.h>

const float PI = 3.14159265358979323846f;

int main (int argc, char** argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface* screen, * image;
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  bool running = true;
  const int FPS = 30;
  Uint32 start;

  SDL_Rect rect;
  rect.x = rect.y = 10;
  rect.w = rect.h = 20;
  Uint32 color = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  //Uint32 color2 = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);

  image = SDL_DisplayFormat(SDL_LoadBMP("img/enemies.bmp"));
  SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0x00, 0xff, 0xff));

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
      default:
	break;
      }
    }
    // Logic & render

    SDL_FillRect(screen, &screen->clip_rect, color);
    //SDL_FillRect(screen, &rect, color2);
    SDL_Rect rect;
    rect.x = 200;
    rect.y = 100;

    SDL_Rect first;

    first.x = first.y = 0;
    first.w = first.h = 128;    
    SDL_BlitSurface(image, &first, screen, NULL);

    first.x = 128;
    SDL_BlitSurface(image, &first, screen, &rect);

    SDL_Flip(screen);

    if (SDL_GetTicks() - start < 1000/FPS)
    {
      SDL_Delay(1000/FPS - (SDL_GetTicks()-start));
    }
  }

  SDL_FreeSurface(image);
  SDL_Quit();
  return 0;
}
