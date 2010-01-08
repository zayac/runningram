/* 
 * File:   Console.cpp
 * Author: necto
 * 
 * Created on January 8, 2010, 12:58 PM
 */

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "initparser.h"
#include "Console.h"
#include "Graphic_subsystem.h"

// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class Console::Initialaiser : public Sectionp
{
public:
	string font_fname;
	int height;
public:

	Initialaiser (string name)
	: Sectionp (name, '='), font_fname ("default.ttf"), height (16)
	{
		Add_param (new St_loader<int > ("font height", &height));
		Add_param (new St_loader<string > ("font file", &font_fname));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>

Console::Console () :cursor_pos(0), parser (new Initialaiser ("[Console]"))
{
	data.push_back (string());
}
//--------------------------------------------------------------------------------------------------
Console::~Console ()
{
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* Console::Get_parser()
{
	return parser;
}
//--------------------------------------------------------------------------------------------------
bool Console::Init()
{
	if (!TTF_WasInit () && TTF_Init() == -1) return false;
	font = TTF_OpenFont (parser->font_fname.c_str(), parser->height);
	return font != 0;
}
//--------------------------------------------------------------------------------------------------
void Console::Cleanup()
{
	if (font) TTF_CloseFont (font);
	if (TTF_WasInit()) TTF_Quit();
}
//--------------------------------------------------------------------------------------------------
void Console::Operate (SDL_KeyboardEvent ev)
{
}
//--------------------------------------------------------------------------------------------------
int DrawText (SDL_Surface* screen, TTF_Font* font, const char* text)
{
    SDL_Color color = {0,200,0};
    SDL_Color color_bg = {0,0,0};
    SDL_Surface *text_surface;

    text_surface = TTF_RenderUTF8_Shaded (font, text, color, color_bg);
    if (text_surface != NULL)
    {
        SDL_BlitSurface(text_surface, NULL, screen, NULL);
        SDL_FreeSurface(text_surface);
        return 1;
    }
    else
    {
        // report error
        return 0;
    }
}

void Console::Draw (Canvas* c) const
{
	SDL_Rect bg = c->Get_screen()->clip_rect;
	bg.h = 200;
	SDL_FillRect (c->Get_screen(), &bg, 132);

	DrawText (c->Get_screen (), font, "Hello, it\'s a simple text вывод in console.");
}
//--------------------------------------------------------------------------------------------------

