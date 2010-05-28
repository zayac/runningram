#include "Rect.h"


//--------------------------------------------------------------------------------------------------
Rect::Rect (int _x, int _y, int _w, int _h) :x(_x), y(_y), w(_w), h(_h){}

Rect::Rect (const Rect& that):x(that.x), y(that.y), w(that.w), h(that.h){}
//--------------------------------------------------------------------------------------------------
Rect& Rect::operator = (const Rect& that) {x = that.x; y = that.y; w = that.w; h = that.h; return *this;}
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
//for realisation of SDL integration look at Canvas.cpp