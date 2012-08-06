#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

void setrects(SDL_Rect* clip)
{
  for (int i = 0; i < 4; i++)
  {
    clip[i].x = 128*i;
    clip[i].y = 0;
    clip[i].w = clip[i].h = 128;
  }
}

int main (int argc, char** argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();

  TTF_Font* font = TTF_OpenFont("typeface/comfortaa/Comfortaa-Regular.ttf", 32);

  SDL_Surface* screen, * image, * text;
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  bool running = true;
  const int FPS = 30;
  Uint32 start;

  int frame = 0;

  SDL_Rect rect;
  rect.x = rect.y = 10;
  rect.w = rect.h = 20;
  Uint32 color = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  //Uint32 color2 = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);

  image = SDL_DisplayFormat(SDL_LoadBMP("img/bird.bmp"));
  SDL_Rect rects[4];
  setrects(rects);

  SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0x00, 0xff, 0xff));

  SDL_Color color3 = {0x66, 0x66, 0x66};
  text = TTF_RenderText_Solid(font, "Hello world!", color3);

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

    SDL_BlitSurface(text, NULL, screen, NULL);

    SDL_BlitSurface(image, &rects[frame], screen, NULL);

    SDL_Flip(screen);

    frame++;
    if (frame >= 4)
    {
      frame = 0;
    }

    if (SDL_GetTicks() - start < 1000/FPS)
    {
      SDL_Delay(1000/FPS - (SDL_GetTicks()-start));
    }
  }

  SDL_FreeSurface(image);
  SDL_FreeSurface(text);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
