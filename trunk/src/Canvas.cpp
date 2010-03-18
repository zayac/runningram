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
#include "Orient.h"

//SDL_Surface* Canvas::screen_p = 0;
//Canvas::base* Canvas::screen_t = 0;

Canvas* Canvas::Screen = 0;

Canvas::Canvas ():UniId<SDL_Surface>(0, 0), pos() { }
//--------------------------------------------------------------------------------------------------
Canvas::Canvas (SDL_Surface* d):UniId<SDL_Surface>(d, 0), pos()
{

}

Canvas::Canvas (char* file):UniId<SDL_Surface>(0, 0), pos()
{
    SDL_Surface *temp = SDL_LoadBMP(file);
    Reinit(SDL_DisplayFormat(temp), 0);
    SDL_FreeSurface(temp);
}
//--------------------------------------------------------------------------------------------------
Canvas::Canvas (const Canvas& orig):UniId<SDL_Surface> (orig.data(), orig.table()), pos()
{

}
//--------------------------------------------------------------------------------------------------
//Canvas::~Canvas () { }
//--------------------------------------------------------------------------------------------------
void Canvas::deleteData()
{
	SDL_FreeSurface (data());
}
//--------------------------------------------------------------------------------------------------
inline int toInt (float a) {return (int)(a + 0.5);}
void Canvas::line (Point start, Point finish, Color c)
{
	start -= pos;
	finish -= pos;
    lineRGBA (data(), start.x, start.y, finish.x, finish.y, c.r, c.g, c.b, c.unused);
}

//--------------------------------------------------------------------------------------------------
void Canvas::fillRect (Rect r, Color col)
{
	r.Move (-pos);
    SDL_FillRect (data(), addSdl (&r), col.Toint (data()));
}
//--------------------------------------------------------------------------------------------------
void Canvas::fill (Color col)
{
	fillRect (Rect (pos.x, pos.y, data()->w, data()->h), col);
}
//--------------------------------------------------------------------------------------------------
Rect Canvas::getClipRect() const
{
	Rect rez = data()->clip_rect;
	rez.Move (pos);
	return rez;
}
//--------------------------------------------------------------------------------------------------
int Canvas::getWidth()
{
	return data()->w;
}
//--------------------------------------------------------------------------------------------------
int Canvas::getHeight()
{
	return data()->h;
}
//--------------------------------------------------------------------------------------------------
void Canvas::copy (Canvas from, Rect src_brd, Point to)
{
	to -= pos;
	SDL_Rect* src_brdp = addSdl (&src_brd);
	if (src_brd.w == 0 && src_brd.h == 0) src_brdp = 0;
	Rect my_brd (to.x, to.y, data()->w - to.x, data()->h - to.y);
	SDL_BlitSurface (from.data(), src_brdp, data(), addSdl (&my_brd));
}
//--------------------------------------------------------------------------------------------------
void Canvas::draw (Canvas* buffer, Point position)
{
    position += pos;
    SDL_Rect dstrect;
    dstrect.x = position.x;
    dstrect.y = position.y;
    // this blits the current frame from the sprite sheet
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = getWidth();
    srcRect.h = getHeight();
    SDL_BlitSurface (data(), &srcRect, buffer->getSurface(), &dstrect);
}
//--------------------------------------------------------------------------------------------------
Canvas Canvas::createCompatible (Point size) const
{
	if (size == Point()) size = Point (data()->w, data()->h);
	return Canvas (SDL_CreateRGBSurface (data()->flags, size.x, size.y, data()->format->BitsPerPixel,
										 data()->format->Rmask, data()->format->Gmask,
                                						 data()->format->Bmask, data()->format->Amask));
}
//--------------------------------------------------------------------------------------------------
void Canvas::update()
{
	SDL_UpdateRect (data(), 0, 0, 0, 0);
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
	return SDL_Color { r, g, b, unused };
}
//--------------------------------------------------------------------------------------------------
Color::Color (const SDL_Color& src) :r (src.r), g (src.g), b (src.b), unused (src.unused){}
//--------------------------------------------------------------------------------------------------
Uint32 Color::Toint (Canvas* screen) const
{
	return SDL_MapRGBA (screen->data()->format, r,g,b,unused);
}
//--------------------------------------------------------------------------------------------------
Uint32 Color::Toint (SDL_Surface* screen) const
{
	return SDL_MapRGBA (screen->format, r,g,b,unused);
}

Color Color::average(vector<Color>* colors)
{
    Uint32 new_r = 0;
    Uint32 new_g = 0;
    Uint32 new_b = 0;
    Uint32 new_t = 0;
    for(int i = 0; i < colors->size(); i++)
    {
        Color c = colors->at(i);
        new_r += c.r;
        new_g += c.g;
        new_b += c.b;
        new_t += c.unused;
    }
    new_r /= colors->size();
    new_g /= colors->size();
    new_b /= colors->size();
    new_t /= colors->size();
    return Color (new_r, new_g, new_b, new_t);
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
	screen->fillRect (*this, c);
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
Color Canvas::getPixel (Point point)
{
	point -= pos;
    Uint32* pixels = (Uint32*)data()->pixels;
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixels[point.y * data()->w + point.x], data()->format, &r, &g, &b, &a);
    return Color (r, g, b, a);
}
//--------------------------------------------------------------------------------------------------
void Canvas::setPixel (Point point, Color pixel)
{
	point -= pos;
    Uint32* pixels = (Uint32*)data()->pixels;
    pixels[point.y * data()->w + point.x] = pixel.Toint(data());
}
//--------------------------------------------------------------------------------------------------
void Canvas::rotate (double angle)
{
	Orient rot (angle);
	pos = rot.Rotate (pos.To<float>()).To <int>();
	Substitute_data (rotozoomSurface(data(), angle, 1, 1));
}
//--------------------------------------------------------------------------------------------------
void Canvas::flipHorizontal()
{
	pos.x = -pos.x;
	Substitute_data (zoomSurface(data(), 1, -1, SMOOTHING_ON));
}
//--------------------------------------------------------------------------------------------------
void Canvas::flipVertical()
{
	pos.y = -pos.y;
	Substitute_data (zoomSurface(data(), -1, 1, SMOOTHING_ON));
}
//--------------------------------------------------------------------------------------------------
void Canvas::zoom (double zoomx, double zoomy)  // percentage to zoom in
{
	pos.x *= zoomx;
	pos.y *= zoomy;
	Substitute_data (zoomSurface (data(), zoomx, zoomy, SMOOTHING_ON));
}
//--------------------------------------------------------------------------------------------------
Canvas Canvas::cropRect (Point point, int w, int h, bool remember_pos)
{
	point -= pos;
    // create a new surface
    int amask = 0;
    if (data()->flags & SDL_SRCCOLORKEY)
	{
        amask = data()->format->Amask;
    }
    Canvas newrect ( SDL_CreateRGBSurface(  SDL_SWSURFACE,
                                            w,
                                            h,
                                            data()->format->BitsPerPixel,
                                            data()->format->Rmask,
                                            data()->format->Gmask,
                                            data()->format->Bmask,
                                            amask ));

    for (int j = 0; j < h; j++)
	{
        for (int i = 0; i < w; i++)
        {
            newrect.setPixel (Point(i, j), this->getPixel (Point (point.x + i, point.y + j)));
        }
    }
    //Copy color key
    if (data()->flags & SDL_SRCCOLORKEY)
	{
        SDL_SetColorKey (newrect.getSurface(), SDL_RLEACCEL|SDL_SRCCOLORKEY, data()->format->colorkey);
    }
	if (remember_pos) newrect.setPos (point + pos);
    return newrect;
}
//--------------------------------------------------------------------------------------------------
void Canvas::setTransparency(Color colorkey)
{
    SDL_SetColorKey( data(), SDL_SRCCOLORKEY|SDL_RLEACCEL, colorkey.Toint(data()) );
	Substitute_data (SDL_DisplayFormat(data()));
//    data() = ;
    //SDL_SetColorKey (data(), SDL_SRCCOLORKEY, colorkey.Toint(data()) );
}
//--------------------------------------------------------------------------------------------------
void Canvas::setTransparentPixel (Point point)
{
	point -= pos;
    setPixel (point, data()->format->colorkey);
}
//--------------------------------------------------------------------------------------------------
bool Canvas::isTransparentPixel (Point point)
{
	point -= pos;
    Uint32 pixelcolor = getPixel(point).Toint(this);
	//test whether pixels color == color of transparent pixels for that surface
	return (pixelcolor == data()->format->colorkey);
}
//--------------------------------------------------------------------------------------------------
Color Canvas::getTransparency()
{
    Uint8 r, g, b, a;
    SDL_GetRGBA(data()->format->colorkey, data()->format, &r, &g, &b, &a);
    return Color (r, g, b, a);
}
//--------------------------------------------------------------------------------------------------
Canvas* Canvas::getScreenCanvas (Point size)
{
	if (Screen == 0)
		Screen = new Canvas (SDL_SetVideoMode (size.x, size.y, 0, 0));
	return Screen;
}
//--------------------------------------------------------------------------------------------------

void Canvas::ortogonalToIsometric()
{
    Canvas new_surf = createCompatible( Point (getWidth() * 4 - 2, getHeight() * 2));
    new_surf.fillRect(Rect (0, 0, new_surf.getWidth(), new_surf.getHeight()), Color (255, 0, 255));
    for (int i = 0; i < getHeight(); i++)
    {
        for(int j = 0; j < 2 * i +1; j++)
        {
            Point new_cell ( new_surf.getWidth() / 2 - i * 2 - 1 + j * 2, i);
            Color color;
            int dj = j / 2;
            if ((j % 2) == 0 )
            {

                color = getPixel( Point (i - dj, dj));
            }
            else
            {
                vector<Color> tmp_vec;
                tmp_vec.push_back( getPixel(Point (i - dj - 1, dj    )));
                tmp_vec.push_back( getPixel(Point (i - dj    , dj    )));
                tmp_vec.push_back( getPixel(Point (i - dj - 1, dj + 1)));
                tmp_vec.push_back( getPixel(Point (i - dj    , dj + 1)));
                color = color.average(&tmp_vec);
            }
            new_surf.setPixel(new_cell, color);
            new_cell.x++;
            new_surf.setPixel(new_cell, color);
        }
    }
    
    for (int i = getHeight() * 2; i > getHeight(); i--)
    {
        for(int j = 0; j < 2 * (getHeight() * 2  - i) + 1; j++)
        {
            Point new_cell ( new_surf.getWidth() / 2 - (getHeight() * 2 - i) * 2 - 1 + j * 2, i - 1);
            Color color;
            int dj = j / 2;
            if ((j % 2) == 0 )
            {

                color = getPixel( Point (getHeight() - dj - 1, i - getHeight() + dj - 1));
            }
            else
            {
                vector<Color> tmp_vec;
                tmp_vec.push_back( getPixel(Point (getHeight() - dj - 1, i - getHeight() + dj - 1)));
                tmp_vec.push_back( getPixel(Point (getHeight() - dj - 2, i - getHeight() + dj    )));
                tmp_vec.push_back( getPixel(Point (getHeight() - dj - 1, i - getHeight() + dj    )));
                tmp_vec.push_back( getPixel(Point (getHeight() - dj - 2, i - getHeight() + dj - 1)));
                color = color.average(&tmp_vec);
            }
            new_surf.setPixel(new_cell, color);
            new_cell.x++;
            new_surf.setPixel(new_cell, color);
        }
    }
	Reinit (new_surf);
    setTransparency(Color (255, 0, 255));
    //SDL_SetColorKey( data(), SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB( data()->format,0, 0, 0 ) );
    //data() = SDL_DisplayFormat(data());

}
