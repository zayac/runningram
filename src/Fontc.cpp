#include <SDL/SDL_ttf.h>
#include "initparser.h"
#include "Fontc.h"
#include "Player_manager.h"

//--------------------------------------------------------------------------------------------------
Fontc::Fontc():UniId<TTF_Font> (0, 0), col (Color()), bgcol (Color()), fq(fqBAD), bg (bgcol), fg (col)
{
}
//--------------------------------------------------------------------------------------------------
Fontc::Fontc (int height, const char* fname, Font_quality fq_, Color f, Color b)
	:UniId<TTF_Font>(TTF_OpenFont (fname, height), 0), col (f), bgcol (b), fq (fq_), bg (bgcol), fg (col)
{
	assert (ok());
}
//--------------------------------------------------------------------------------------------------
Fontc::Fontc (const Fontc& orig):UniId<TTF_Font> (orig),
	bgcol (orig.bgcol), col (orig.col), fq (orig.fq), bg(bgcol), fg (col)
{
	assert (ok());
}
//--------------------------------------------------------------------------------------------------
Fontc::~Fontc()
{
	destroy();
}
//--------------------------------------------------------------------------------------------------
void Fontc::deleteData()
{
	if (data())
		TTF_CloseFont (data());
}
//--------------------------------------------------------------------------------------------------
void Fontc::openfont (const char* fname, int height)
{
	if (!TTF_WasInit () && TTF_Init() == -1) return;

	reinit (TTF_OpenFont (fname, height), 0);
}
//--------------------------------------------------------------------------------------------------
Fontc& Fontc::operator= (const Fontc& orig)
{
	UniId<TTF_Font>::operator = (orig);
	col = orig.col;
	bgcol = orig.bgcol;
	fq = orig.fq;
}
//--------------------------------------------------------------------------------------------------
Canvas Fontc::createLabel (const char* line, bool color_reverse, Rect* brd) const
{
    assert(ok());
    SDL_Surface *text_surface;
	if (color_reverse)
		switch (fq)
		{
		case fqBAD:
			text_surface = TTF_RenderUTF8_Solid (data(), line, bgcol);
			break;
		case fqGOOD:
			text_surface = TTF_RenderUTF8_Shaded (data(), line, bgcol, col);
			break;
		case fqTRANSPARENT:
			text_surface = TTF_RenderUTF8_Blended (data(), line, bgcol);
			break;
		}
	else
		switch (fq)
		{
		case fqBAD:
			text_surface = TTF_RenderUTF8_Solid (data(), line, col);
			break;
		case fqGOOD:
			text_surface = TTF_RenderUTF8_Shaded (data(), line, col, bgcol);
			break;
		case fqTRANSPARENT:
			text_surface = TTF_RenderUTF8_Blended (data(), line, col);
			break;
		}
	Canvas ret (text_surface);
	if (brd && ret.ok()) ret.setPos (Point (brd->x, brd->y));

	if (brd && ret.ok() && (brd->w < text_surface->w || brd->h < text_surface->h))
	{
		ret.cropRect (Point (brd->x, brd->y), min(brd->w, text_surface->w), min(brd->h, text_surface->h), true);
	}

	return ret;
}
//--------------------------------------------------------------------------------------------------
int Fontc::drawLine (Canvas* screen, const char* line, Rect* brd, bool color_reverse) const
{
    assert(ok());

	if (line[0] == '\0') return 0;
	Canvas txt = createLabel (line, color_reverse);
	if (!txt.ok()) return 0;

	Rect real_brd;
	if (brd) real_brd = *brd;
	else
		real_brd = txt.getClipRect ();

	screen->copy (txt, Point(), real_brd);
	return min(txt.getHeight (), real_brd.y);
}
//--------------------------------------------------------------------------------------------------
Point Fontc::strSize (const char* str) const
{
	assert(ok());
	int w = 0, h = 0;
	TTF_SizeUTF8 (data(), str, &w, &h);
	return Point (w, h);
}
//--------------------------------------------------------------------------------------------------
int Fontc::height() const
{
	assert(ok());
	return TTF_FontHeight (data());
}
//--------------------------------------------------------------------------------------------------
int Fontc::approximateNumSymbols (int width) const
{
	assert(ok());
	int w = strLen ("-");//Get a size of two typically characters
	return width/w; //and measure by it
}
//--------------------------------------------------------------------------------------------------
bool Fontc::ok() const
{
	return UniId<TTF_Font>::ok();
}
//--------------------------------------------------------------------------------------------------
void FontcCleanUp()
{
	if (TTF_WasInit()) TTF_Quit();
}