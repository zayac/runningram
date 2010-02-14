/* 
 * File:   Canvas.cpp
 * Author: necto
 * 
 * Created on January 10, 2010, 2:36 PM
 */

#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include "Canvas.h"
#include "Battlefield.h"


SDL_Surface* Canvas::screen_p = 0;
Canvas::base* Canvas::screen_t = 0;

Canvas::Canvas ():UniId<SDL_Surface>(0, 0) { }
//--------------------------------------------------------------------------------------------------
Canvas::Canvas (SDL_Surface* d):UniId<SDL_Surface>(d, 0)
{

}

Canvas::Canvas (char* file):UniId<SDL_Surface>(0, 0)
{
    SDL_Surface *temp = SDL_LoadBMP(file);
    Reinit(SDL_DisplayFormat(temp), 0);
    SDL_FreeSurface(temp);
}
//--------------------------------------------------------------------------------------------------
Canvas::Canvas (const Canvas& orig):UniId<SDL_Surface> (orig.data, orig.table)
{

}
//--------------------------------------------------------------------------------------------------
//Canvas::~Canvas () { }
//--------------------------------------------------------------------------------------------------
void Canvas::deleteData()
{
	if (data != screen_p)
		SDL_FreeSurface (data);
	else
		screen_p = 0;
}
//--------------------------------------------------------------------------------------------------
inline int toInt (float a) {return (int)(a + 0.5);}
void Canvas::line (Point start, Point finish, Color c)
{   
    lineRGBA (data, start.x, start.y, finish.x, finish.y, c.r, c.g, c.b, c.unused);
}

//--------------------------------------------------------------------------------------------------
void Canvas::fillRect (Rect r, Color col)
{
    SDL_FillRect (data, addSdl (&r), col.Toint (data));
}
//--------------------------------------------------------------------------------------------------
void Canvas::fill (Color col)
{
	fillRect (Rect (0, 0, data->w, data->h), col);
}
//--------------------------------------------------------------------------------------------------
Rect Canvas::getClipRect() const
{
	return data->clip_rect;
}
//--------------------------------------------------------------------------------------------------
int Canvas::getWidth()
{
	return data->w;
}
//--------------------------------------------------------------------------------------------------
int Canvas::getHeight()
{
	return data->h;
}
//--------------------------------------------------------------------------------------------------
void Canvas::copy (Canvas from, Rect src_brd, Point to)
{
	SDL_Rect* src_brdp = addSdl (&src_brd);
	if (src_brd.w == 0 && src_brd.h == 0) src_brdp = 0;
	Rect my_brd (to.x, to.y, data->w - to.x, data->h - to.y);
	SDL_BlitSurface (from.data, src_brdp, data, addSdl (&my_brd));
}
//--------------------------------------------------------------------------------------------------
Canvas Canvas::createCompatible (Point size) const
{
	if (size == Point()) size = Point (data->w, data->h);
	return Canvas (SDL_CreateRGBSurface (data->flags, size.x, size.y, data->format->BitsPerPixel,
										 data->format->Rmask, data->format->Gmask,
                                						 data->format->Bmask, data->format->Amask));
}
//--------------------------------------------------------------------------------------------------
bool Canvas::getFromScreen (Point size)
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
void Canvas::update()
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
Color::operator SDL_Color() const
{
	return SDL_Color {r, g, b, unused};
}
//--------------------------------------------------------------------------------------------------
Color::Color (const SDL_Color& src) :r (src.r), g (src.g), b (src.b), unused (src.unused){}
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
Rect::Rect (int _x, int _y, int _w, int _h) :x(_x), y(_y), w(_w), h(_h){}

Rect::Rect (const Rect& that):x(that.x), y(that.y), w(that.w), h(that.h){}
//--------------------------------------------------------------------------------------------------
Rect& Rect::operator = (const Rect& that) {x = that.x; y = that.y; w = that.w; h = that.h; return *this;}
//--------------------------------------------------------------------------------------------------
Rect::Rect (const SDL_Rect& that):x(that.x), y(that.y), w(that.w), h(that.h){}
//--------------------------------------------------------------------------------------------------
Rect::operator SDL_Rect() const {return SDL_Rect {x, y, w, h};}
//--------------------------------------------------------------------------------------------------
void Rect::Draw (Canvas* screen, Color c) const
{
	SDL_Rect copy(*this);
	SDL_FillRect (screen->getSurface(), &copy, c.Toint (screen->getSurface()));
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
Color Canvas::getPixel(Point point)
{
    Uint32* pixels = (Uint32*)data->pixels;
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixels[point.y * data->w + point.x], data->format, &r, &g, &b, &a);
    return Color (r, g, b, a);
}
//--------------------------------------------------------------------------------------------------
void Canvas::setPixel(Point point, Color pixel)
{
    Uint32* pixels = (Uint32*)data->pixels;
    pixels[point.y * data->w + point.x] = pixel.Toint(data);
}
//--------------------------------------------------------------------------------------------------
void Canvas::rotate(double angle)
{
	data = rotozoomSurface(data, angle, 1, 1);
}
//--------------------------------------------------------------------------------------------------
void Canvas::flipHorizontal()
{
	data = zoomSurface(data, 1, -1, SMOOTHING_ON);
}
//--------------------------------------------------------------------------------------------------
void Canvas::flipVertical()
{
	data = zoomSurface(data, -1, 1, SMOOTHING_ON);
}
//--------------------------------------------------------------------------------------------------
void Canvas::zoom(double zoomx, double zoomy)  // percentage to zoom in
{
	data = zoomSurface (data, zoomx, zoomy, SMOOTHING_ON);
}
//--------------------------------------------------------------------------------------------------
Canvas Canvas::cropRect(Point point, int w, int h)
{
    // create a new surface
    int amask = 0;
    if(data->flags & SDL_SRCCOLORKEY)
	{
        amask = data->format->Amask;
    }
    Canvas newrect ( SDL_CreateRGBSurface(  SDL_SWSURFACE,
                                            w,
                                            h,
                                            data->format->BitsPerPixel,
                                            data->format->Rmask,
                                            data->format->Gmask,
                                            data->format->Bmask,
                                            amask ));

    for(int j = 0; j < h; j++)
	{
        for(int i = 0; i < w; i++)
        {
            newrect.setPixel(Point(i, j), this->getPixel(Point (point.x + i,point.y + j)));
        }
    }
    //Copy color key
    if(data->flags & SDL_SRCCOLORKEY)
	{
        SDL_SetColorKey(newrect.getSurface(), SDL_RLEACCEL|SDL_SRCCOLORKEY, data->format->colorkey );
    }
    return newrect;
}
//--------------------------------------------------------------------------------------------------
void Canvas::setTransparency(Color colorkey)
{
    SDL_SetColorKey(data, SDL_SRCCOLORKEY, colorkey.Toint(data) );
}
//--------------------------------------------------------------------------------------------------
void Canvas::setTransparentPixel(Point point)
{
    setPixel(point, data->format->colorkey);
}
//--------------------------------------------------------------------------------------------------
bool Canvas::isTransparentPixel(Point point)
{
    Uint32 pixelcolor = getPixel(point).Toint(this);
	//test whether pixels color == color of transparent pixels for that surface
	return (pixelcolor == data->format->colorkey);
}
//--------------------------------------------------------------------------------------------------
void Canvas::draw(Canvas* buffer, Point pos)
{
    SDL_Rect dstrect;
    dstrect.x = pos.x;
    dstrect.y = pos.y;
    // this blits the current frame from the sprite sheet
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = getWidth();
    srcRect.h = getHeight();
    SDL_BlitSurface(data, &srcRect, buffer->getSurface(), &dstrect);
}
//--------------------------------------------------------------------------------------------------
Color Canvas::getTransparency()
{
    Uint8 r, g, b, a;
    SDL_GetRGBA(data->format->colorkey, data->format, &r, &g, &b, &a);
    return Color (r, g, b, a);
}