/* 
 * File:   Eventman.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 10:27 PM
 */
#include <SDL/SDL.h>
#include "Eventman.h"

Eventman::Eventman () :stop (false)
{
}
//--------------------------------------------------------------------------------------------------
Eventman::~Eventman () { }
//--------------------------------------------------------------------------------------------------
void Eventman::Acts()
{
    SDL_Event event;
	/* look for an event */
	if (SDL_PollEvent (&event))
	{
		/* an event was found */
		switch (event.type)
		{
			/* close button clicked */
		case SDL_QUIT:
			stop = 1;
			break;

			/* handle the keyboard */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_q:
				stop = 1;
				break;
			}
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------
bool Eventman::Stopped() const
{
	return stop;
}
//--------------------------------------------------------------------------------------------------
bool Eventman::Ok() const
{
	return true;
}
//--------------------------------------------------------------------------------------------------