/* 
 * File:   Canvas.cpp
 * Author: necto
 * 
 * Created on January 10, 2010, 2:36 PM
 */

#include <assert.h>
#include "Canvas.h"

Canvas::Canvas () { }
//--------------------------------------------------------------------------------------------------
//Canvas::~Canvas () { }
//--------------------------------------------------------------------------------------------------
inline int To_int (float a) {return (int)(a + 0.5);}
void Canvas::Line (Point start, Point finish, Color c)
{
	Point delta;
	if (!In_brd (start, finish, delta)) return;

	assert(0 <= start.y && start.y < h);
	assert(0 <= finish.y && finish.y < h);
	assert(0 <= start.x && start.x < w);
	assert(0 <= finish.x && finish.x < w);

	Uint32 col = c.Toint (this);
	if (delta.x >= abs(delta.y))
	{
		int x = start.x;
		float y = start.y;
		float inc = ((float)delta.y)/delta.x;
		for (; x <= finish.x; ++x, y += inc)
			Set_pixel (x, To_int (y), col);

		if (finish.y > start.y)
			 SDL_UpdateRect (this, start.x, start.y, finish.x - start.x, finish.y - start.y);
		else SDL_UpdateRect (this, start.x, finish.y, finish.x - start.x, start.y - finish.y);
	}
	else if (delta.y >= abs(delta.x))
	{
		float x = start.x;
		int y = start.y;
		float inc = ((float)delta.x)/delta.y;
		for (; y <= finish.y; ++y, x += inc)
			Set_pixel ((int)x, To_int (y), col);

		if (finish.x > start.x)
			 SDL_UpdateRect (this, start.x, start.y, finish.x - start.x, finish.y - start.y);
		else SDL_UpdateRect (this, finish.x, start.y, start.x - finish.x, finish.y - start.y);
	}
	else return Line (finish, start, c);
}
//--------------------------------------------------------------------------------------------------
void Canvas::Copy (Canvas* from, Rect src_brd, Point to)
{
	Rect* src_brdp = &src_brd;
	if (src_brd.w == 0 && src_brd.h == 0) src_brdp = 0;
	Rect my_brd (to.x, to.y, w - to.x, h - to.y);
	SDL_BlitSurface (from, src_brdp, this, &my_brd);
}
//--------------------------------------------------------------------------------------------------
Canvas* Canvas::Create_compatible (Point size) const
{
	if (size == Point()) size = Point (w, h);
	return (Canvas*) SDL_CreateRGBSurface (flags, size.x, size.y, format->BitsPerPixel, format->Rmask, format->Gmask, format->Bmask, format->Amask);
}
//--------------------------------------------------------------------------------------------------
void Canvas::Update()
{
	SDL_UpdateRect (this, 0, 0, 0, 0);
}
//--------------------------------------------------------------------------------------------------
bool Canvas::May_not_draw (Point& start, Point& finish) const
{
	return	(start.x >= w && finish.x >= w) ||
			(start.x < 0 && finish.x < 0) ||
			(start.y >= h && finish.y >= h) ||
			(start.y < 0 && finish.y < 0);
}
//--------------------------------------------------------------------------------------------------
bool Canvas::May_draw (Point& start, Point& finish) const
{
	return	(start.x < w && finish.x < w) &&
			(start.x >= 0 && finish.x >= 0) &&
			(start.y < h && finish.y < h) &&
			(start.y >= 0 && finish.y >= 0);
}
//--------------------------------------------------------------------------------------------------
bool Canvas::In_brd (Point& start, Point& finish, Point& delta) const
{
	if (May_not_draw (start, finish)) return false;

	delta = finish - start;
	if (finish.x >= w)	delta = delta*(w - 1 - start.x)/delta.x;
	if (finish.y >= h && delta.x != 0)	delta = delta*(h - 1 - start.y)/delta.y;
	if (finish.x < 0 && delta.x != 0)	delta = -delta*start.x/delta.x;
	if (finish.y < 0 && delta.y != 0)	delta = -delta*start.y/delta.y;
	finish = start + delta;

	if (May_not_draw (start, finish)) return false;

	if (start.x >= w)	delta = -delta*(w - 1 - finish.x)/delta.x;
	if (start.y >= h && delta.x != 0)	delta = -delta*(h - 1 - finish.y)/delta.y;
	if (start.x < 0 && delta.x != 0)	delta = delta*finish.x/delta.x;
	if (start.y < 0 && delta.y != 0)	delta = delta*finish.y/delta.y;
	start = finish - delta;

	return May_draw (start, finish);
}
//--------------------------------------------------------------------------------------------------
bool Canvas::Ok() const
{
	return true;
}
//--------------------------------------------------------------------------------------------------
Uint32 Color::Toint (SDL_Surface* screen) const
{
	return SDL_MapRGB (screen->format, r, g, b);
}
//--------------------------------------------------------------------------------------------------
Rect::Rect (int _x, int _y, int _w, int _h) {x = _x; y = _y; w = _w; h = _h;}

Rect::Rect (const Rect& that) {x = that.x; y = that.y; w = that.w; h = that.h;}
//--------------------------------------------------------------------------------------------------
Rect& Rect::operator = (const Rect& that) {x = that.x; y = that.y; w = that.w; h = that.h; return *this;}
//--------------------------------------------------------------------------------------------------
Rect::Rect (const SDL_Rect& that) {x = that.x; y = that.y; w = that.w; h = that.h;}
//--------------------------------------------------------------------------------------------------
void Rect::Draw (SDL_Surface* screen, Color c) const
{
	Rect copy(*this);
	SDL_FillRect (screen, &copy, c.Toint (screen));
}
//--------------------------------------------------------------------------------------------------
void Rect::Cut_left (int what)
{
	x += what;
	w -= what;
}
//--------------------------------------------------------------------------------------------------
void Rect::Cut_top (int what)
{
	y += what;
	h -= what;
}
//--------------------------------------------------------------------------------------------------
