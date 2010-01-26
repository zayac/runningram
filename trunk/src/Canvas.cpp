/* 
 * File:   Canvas.cpp
 * Author: necto
 * 
 * Created on January 10, 2010, 2:36 PM
 */

#include <assert.h>
//#include <SDL/SDL_gfx.h>
#include "Canvas.h"
#include "Battlefield.h"


SDL_Surface* Canvas::screen_p = 0;
Canvas::base* Canvas::screen_t = 0;

Canvas::Canvas ():UniId<SDL_Surface>(0, 0) { }
//--------------------------------------------------------------------------------------------------
Canvas::Canvas (SDL_Surface* d):UniId<SDL_Surface>(d, 0)
{

}
//--------------------------------------------------------------------------------------------------
Canvas::Canvas (const Canvas& orig):UniId<SDL_Surface> (orig.data, orig.table)
{

}
//--------------------------------------------------------------------------------------------------
//Canvas::~Canvas () { }
//--------------------------------------------------------------------------------------------------
void Canvas::Delete_data()
{
	if (data != screen_p)
		SDL_FreeSurface (data);
	else
		screen_p = 0;
}
//--------------------------------------------------------------------------------------------------
inline int To_int (float a) {return (int)(a + 0.5);}
void Canvas::Line (Point start, Point finish, Color c)
{   
    lineRGBA(data, start.x, start.y, finish.x, finish.y, c.r, c.g, c.b, c.unused);
}

//--------------------------------------------------------------------------------------------------
void Canvas::Fill_rect (Rect r, Color col)
{
    SDL_FillRect (data, &r, col.Toint (data));
}
//--------------------------------------------------------------------------------------------------
void Canvas::Copy (Canvas from, Rect src_brd, Point to)
{
	Rect* src_brdp = &src_brd;
	if (src_brd.w == 0 && src_brd.h == 0) src_brdp = 0;
	Rect my_brd (to.x, to.y, data->w - to.x, data->h - to.y);
	SDL_BlitSurface (from.data, src_brdp, data, &my_brd);
}
//--------------------------------------------------------------------------------------------------
Canvas Canvas::Create_compatible (Point size) const
{
	if (size == Point()) size = Point (data->w, data->h);
	return Canvas (SDL_CreateRGBSurface (data->flags, size.x, size.y, data->format->BitsPerPixel,
										 data->format->Rmask, data->format->Gmask,
										 data->format->Bmask, data->format->Amask));
}
//--------------------------------------------------------------------------------------------------
bool Canvas::Get_from_screen (Point size)
{
	if (screen_t == 0) screen_t = new base;
	if (screen_p == 0)
	{
		screen_p = SDL_SetVideoMode (size.x, size.y, 0, 0);
		Reinit (screen_p, screen_t);
		return true;
	}
	else
		Reinit (0, 0);//if screen has allready getted you need copy that Canvas
	return false;
}
//--------------------------------------------------------------------------------------------------
void Canvas::Update()
{
	SDL_UpdateRect (data, 0, 0, 0, 0);
}
//--------------------------------------------------------------------------------------------------
//bool Canvas::May_not_draw (Point& start, Point& finish) const
//{
//	return	(start.x >= w && finish.x >= w) ||
//			(start.x < 0 && finish.x < 0) ||
//			(start.y >= h && finish.y >= h) ||
//			(start.y < 0 && finish.y < 0);
//}
////--------------------------------------------------------------------------------------------------
//bool Canvas::May_draw (Point& start, Point& finish) const
//{
//	return	(start.x < w && finish.x < w) &&
//			(start.x >= 0 && finish.x >= 0) &&
//			(start.y < h && finish.y < h) &&
//			(start.y >= 0 && finish.y >= 0);
//}
////--------------------------------------------------------------------------------------------------
//bool Canvas::In_brd (Point& start, Point& finish, Point& delta) const
//{
//	if (May_not_draw (start, finish)) return false;
//
//	delta = finish - start;
//	if (finish.x >= w)	delta = delta*(w - 1 - start.x)/delta.x;
//	if (finish.y >= h && delta.x != 0)	delta = delta*(h - 1 - start.y)/delta.y;
//	if (finish.x < 0 && delta.x != 0)	delta = -delta*start.x/delta.x;
//	if (finish.y < 0 && delta.y != 0)	delta = -delta*start.y/delta.y;
//	finish = start + delta;
//
//	if (May_not_draw (start, finish)) return false;
//
//	if (start.x >= w)	delta = -delta*(w - 1 - finish.x)/delta.x;
//	if (start.y >= h && delta.x != 0)	delta = -delta*(h - 1 - finish.y)/delta.y;
//	if (start.x < 0 && delta.x != 0)	delta = delta*finish.x/delta.x;
//	if (start.y < 0 && delta.y != 0)	delta = delta*finish.y/delta.y;
//	start = finish - delta;
//
//	return May_draw (start, finish);
//}
//--------------------------------------------------------------------------------------------------
bool Canvas::Ok() const
{
	return UniId<SDL_Surface>::Ok();
}
//--------------------------------------------------------------------------------------------------
Uint32 Color::Toint (Canvas* screen) const
{
	return SDL_MapRGBA (screen->data->format, r,g,b,unused);
}
//--------------------------------------------------------------------------------------------------
Uint32 Color::Toint (SDL_Surface* screen) const
{
	return SDL_MapRGBA (screen->format, r,g,b,unused);
}
//--------------------------------------------------------------------------------------------------
Rect::Rect (int _x, int _y, int _w, int _h) {x = _x; y = _y; w = _w; h = _h;}

Rect::Rect (const Rect& that) {x = that.x; y = that.y; w = that.w; h = that.h;}
//--------------------------------------------------------------------------------------------------
Rect& Rect::operator = (const Rect& that) {x = that.x; y = that.y; w = that.w; h = that.h; return *this;}
//--------------------------------------------------------------------------------------------------
Rect::Rect (const SDL_Rect& that) {x = that.x; y = that.y; w = that.w; h = that.h;}
//--------------------------------------------------------------------------------------------------
void Rect::Draw (Canvas* screen, Color c) const
{
	Rect copy(*this);
	SDL_FillRect (screen->data, &copy, c.Toint (screen));
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
