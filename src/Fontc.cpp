#include <SDL/SDL_ttf.h>
#include "Fontc.h"

//--------------------------------------------------------------------------------------------------

Fontc::Fontc():UniId<TTF_Font> (0, 0), col (Color()), bgcol (Color()), bg (bgcol), fg (col)
{
}
//--------------------------------------------------------------------------------------------------
Fontc::Fontc (int height, const char* fname, Color f, Color b)
	:UniId<TTF_Font>(TTF_OpenFont (fname, height), 0), col (f), bgcol (b), bg (bgcol), fg (col)
{
	assert (Ok());
}
//--------------------------------------------------------------------------------------------------
Fontc::Fontc (const Fontc& orig):UniId<TTF_Font> (orig), bgcol (orig.bgcol), col (orig.col), bg(bgcol), fg (col)
{
	assert (Ok());
}
//--------------------------------------------------------------------------------------------------
Fontc::~Fontc()
{
}
//--------------------------------------------------------------------------------------------------
void Fontc::Delete_data()
{
	if (data)
		TTF_CloseFont (data);
}
//--------------------------------------------------------------------------------------------------
void Fontc::Open_font (const char* fname, int height)
{
	if (!TTF_WasInit () && TTF_Init() == -1) return;

	Reinit (TTF_OpenFont (fname, height), 0);
}
//--------------------------------------------------------------------------------------------------
Fontc& Fontc::operator= (const Fontc& orig)
{
	UniId<TTF_Font>::operator = (orig);
	col = orig.col;
	bgcol = orig.bgcol;
}
//--------------------------------------------------------------------------------------------------
int Fontc::Draw_line (Canvas* screen, const char* line, Rect* brd, bool color_reverse) const
{
	assert(Ok());
    SDL_Surface *text_surface;
	if (color_reverse)
		text_surface = TTF_RenderUTF8_Shaded (data, line, bgcol, col);
	else
		text_surface = TTF_RenderUTF8_Shaded (data, line, col, bgcol);

	Point size;
    if (text_surface != NULL)
    {
		size = Str_size (line);

		Rect src_brd = *brd;
		src_brd.x = 0;
		src_brd.y = 0;
		if (src_brd.w > size.x) src_brd.w = size.x;
		if (src_brd.h > size.y) src_brd.h = size.y;

		brd->x -= screen->getPos ().x;
		brd->y -= screen->getPos ().y;
        SDL_BlitSurface (text_surface, addSdl (&src_brd), screen->data, addSdl (brd));
        SDL_FreeSurface (text_surface);

        return size.y;
    }
	return 0;
}
//--------------------------------------------------------------------------------------------------
Point Fontc::Str_size (const char* str) const
{
	assert(Ok());
	int w = 0, h = 0;
	TTF_SizeUTF8 (data, str, &w, &h);
	return Point (w, h);
}
//--------------------------------------------------------------------------------------------------
int Fontc::Height() const
{
	assert(Ok());
	return TTF_FontHeight (data);
}
//--------------------------------------------------------------------------------------------------
int Fontc::Approximate_num_symbols (int width) const
{
	assert(Ok());
	int w = Str_len ("-");//Get a size of two typically characters
	return width/w; //and measure by it
}
//--------------------------------------------------------------------------------------------------
bool Fontc::Ok() const
{
	return UniId<TTF_Font>::Ok();
}
//--------------------------------------------------------------------------------------------------
void FontcCleanUp()
{
	if (TTF_WasInit()) TTF_Quit();
}