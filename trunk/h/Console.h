/* 
 * File:   Console.h
 * Author: necto
 *
 * Created on January 8, 2010, 12:58 PM
 */

#ifndef _CONSOLE_H
#define	_CONSOLE_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <list>
#include <string>
#include "Graphic_subsystem.h"

using std::string;
using std::list;

class Console
{
	TTF_Font* font;
	list <string> data;
	int cursor_pos;

	class Initialaiser;
	Initialaiser* parser;

public:
	Console();
	virtual ~Console();

	Serializator* Get_parser();
	bool Init();
	void Cleanup();

	void Operate (SDL_KeyboardEvent ev);
	void Draw (Canvas* c) const;
};

#endif	/* _CONSOLE_H */

