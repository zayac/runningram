/* 
 * File:   Fontc.h
 * Author: necto
 *
 * Created on February 14, 2010, 9:39 PM
 */

#ifndef _FONTC_H
#define	_FONTC_H

#include <assert.h>
#include "Canvas.h"
#include "UniId.h"

#ifndef _SDL_TTF_H
class TTF_Font;
#endif

class Fontc :public UniId<TTF_Font>
{
	Color col;
	Color bgcol;

	virtual void Delete_data();

public:
	Fontc ();
	Fontc (int height, const char* fname, Color fg = Color(), Color bg = Color());
	Fontc (const Fontc& orig);
	~Fontc();

	Fontc& operator= (const Fontc& orig);
	void Open_font (const char* fname, int height);

	void Set_bg (const Color& b) {bgcol = b;}
	void Set_fg (const Color& f) {col = f;}


	const Color& bg;
	const Color& fg;

	Point Str_size (const char* str) const;
	inline int Str_height (const char* str) const {return Str_size (str).y;}
	inline int Str_len (const char* str) const    {return Str_size (str).x;}
	int Height() const;
	int Approximate_num_symbols (int width) const;
	int Draw_line (Canvas* screen, const char* line, Rect* brd, bool color_reverse = false) const;

	bool Ok() const;
};

void FontcCleanUp();

#endif	/* _FONTC_H */

