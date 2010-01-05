#include "Init_client.h"
#include <iostream>
#include "SDL.h"

using std::cout;
using std::cin;
using std::endl;

int main ( int argc, char *argv[] )
{
    Init_client ic ("gensec");
    File_loader fl ("./settings.cfg");
    fl.Read_sector (&ic);
    cout <<ic.win_name;

  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);

  /* set the title bar */
  SDL_WM_SetCaption ("dhff", "name");

  /* create window */
  SDL_Surface* screen = SDL_SetVideoMode (640, 480, 0, 0);

  /* load bitmap to temp surface */
  SDL_Surface* temp = SDL_LoadBMP("sdl_logo.bmp");

  /* convert bitmap to display format */
  SDL_Surface* bg = SDL_DisplayFormat(temp);

  /* free the temp surface */
  SDL_FreeSurface(temp);

  SDL_Event event;
  int gameover = 0;

  /* message pump */
  while (!gameover)
  {
    /* look for an event */
    if (SDL_PollEvent(&event)) {
      /* an event was found */
      switch (event.type) {
        /* close button clicked */
        case SDL_QUIT:
          gameover = 1;
          break;

        /* handle the keyboard */
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
            case SDLK_q:
              gameover = 1;
              break;
          }
          break;
      }
    }

    /* draw the background */
    SDL_BlitSurface(bg, NULL, screen, NULL);

    /* update the screen */
    SDL_UpdateRect(screen, 0, 0, 0, 0);
  }

  /* free the background surface */
  SDL_FreeSurface(bg);

  /* cleanup SDL */
  SDL_Quit();

  return 0;
}