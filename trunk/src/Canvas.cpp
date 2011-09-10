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
#include <SDL/SDL_image.h>
#include "Canvas.h"
#include "Battlefield.h"
#include "Orient.h"
#include "Exception.h"
#include "Logger.h"

Canvas* Canvas::Screen = 0;

Canvas::Canvas ():UniId<SDL_Surface>(0, 0), pos() { }
//--------------------------------------------------------------------------------------------------
Canvas::Canvas (SDL_Surface* d):UniId<SDL_Surface>(d, 0), pos() { }

Canvas::Canvas (const char* file, bool alpha):UniId<SDL_Surface>(0, 0), pos()
{
    SDL_Surface *temp = NULL;
    temp = IMG_Load (file);
    if (temp == NULL)
    {
    	LOG(ERROR) <<"Can't load file " << file <<"while initialising canvas";
    	throw Exception (Sprintf ("can't load file %s !", file));
    }

    if (alpha)
        reinit (SDL_DisplayFormatAlpha(temp), 0);
    else
        reinit (SDL_DisplayFormat(temp), 0);
    SDL_FreeSurface (temp);
    //SDL_SetColorKey( this->data(), SDL_SRCCOLORKEY, SDL_MapRGB( this->data()->format, 0, 0xFF, 0xFF ) );
}
//--------------------------------------------------------------------------------------------------
Canvas::Canvas (const Canvas& orig):UniId<SDL_Surface> (orig.data(), orig.table()), pos (orig.pos)
{

}
//--------------------------------------------------------------------------------------------------
Canvas::~Canvas ()
{
	destroy ();
}
//--------------------------------------------------------------------------------------------------
void Canvas::deleteData()
{
	if (data())
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
	r.move (-pos);
    SDL_FillRect (data(), addSdl (&r), col.toInt (data()));
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
	rez.move (pos);
	return rez;
}
//--------------------------------------------------------------------------------------------------
int Canvas::getWidth() const
{
	return data()->w;
}
//--------------------------------------------------------------------------------------------------
int Canvas::getHeight() const
{
	return data()->h;
}
//--------------------------------------------------------------------------------------------------
void Canvas::copy (const Canvas from, Rect src_brd, Point to)
{
	to += from.pos - pos;
	SDL_Rect* src_brdp = addSdl (&src_brd);
	if (src_brd.w == 0 && src_brd.h == 0) src_brdp = 0;
	Rect my_brd (to.x, to.y, data()->w - to.x, data()->h - to.y);
	SDL_BlitSurface (from.data(), src_brdp, data(), addSdl (&my_brd));
}
//--------------------------------------------------------------------------------------------------
void Canvas::pushPos (Point npos)
{
	posStack.push_back(pos);
	pos = npos;
}
//--------------------------------------------------------------------------------------------------
void Canvas::popPos()
{
	assert(posStack.size() > 0);
	pos = posStack.back();
	posStack.pop_back();
}
//--------------------------------------------------------------------------------------------------
void Canvas::draw (Canvas where, Point position) const
{
    position += pos - where.pos;
    SDL_Rect dstrect;
    dstrect.x = position.x;
    dstrect.y = position.y;
    // this blits the current frame from the sprite sheet
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = getWidth();
    srcRect.h = getHeight();
    SDL_BlitSurface (data(), &srcRect, where.getSurface(), &dstrect);
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
bool Canvas::ok() const
{
	return UniId<SDL_Surface>::ok();
}
//--------------------------------------------------------------------------------------------------
Color::operator SDL_Color() const
{
    SDL_Color tmp;
    tmp.r = r;
    tmp.g = g;
    tmp.b = b;
    tmp.unused = unused;
    return tmp;
}
//--------------------------------------------------------------------------------------------------
Color::Color (const SDL_Color& src) :r (src.r), g (src.g), b (src.b), unused (src.unused){}
//--------------------------------------------------------------------------------------------------
Uint32 Color::Toint (const Canvas* screen) const
{
	return SDL_MapRGBA (screen->data()->format, r,g,b,unused);
}
//--------------------------------------------------------------------------------------------------
Uint32 Color::toInt (const SDL_Surface* screen) const
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
Color Canvas::getPixel (Point point) const
{
	point -= pos;
    Uint32* pixels = (Uint32*)data()->pixels;
    Uint8 r, g, b, a;
	assert(point.y * data()->w + point.x < data()->w*data()->h);
	assert (point.y * data()->w + point.x >= 0);
    SDL_GetRGBA(pixels[point.y * data()->w + point.x], data()->format, &r, &g, &b, &a);
    return Color (r, g, b, a);
}
//--------------------------------------------------------------------------------------------------
void Canvas::setPixel (Point point, Color pixel)
{
	point -= pos;
    Uint32* pixels = (Uint32*)data()->pixels;
    pixels[point.y * data()->w + point.x] = pixel.toInt(data());
}
//--------------------------------------------------------------------------------------------------
void Canvas::rotate (double angle)
{
	Orient rot (angle);
	pos = rot.rotate (pos.to<float>()).to <int>();
	substituteData (rotozoomSurface(data(), angle, 1, 1));
}
//--------------------------------------------------------------------------------------------------
void Canvas::flipHorizontal()
{
	pos.x = -pos.x;
	substituteData (zoomSurface(data(), 1, -1, SMOOTHING_ON));
}
//--------------------------------------------------------------------------------------------------
void Canvas::flipVertical()
{
	pos.y = -pos.y;
	substituteData (zoomSurface(data(), -1, 1, SMOOTHING_ON));
}
//--------------------------------------------------------------------------------------------------
void Canvas::zoom (double zoomx, double zoomy)  // percentage to zoom in
{
	pos.x *= zoomx;
	pos.y *= zoomy;
	substituteData (zoomSurface (data(), zoomx, zoomy, SMOOTHING_OFF));
}
//--------------------------------------------------------------------------------------------------
Canvas Canvas::cropRect (Point point, int w, int h, bool remember_pos)
{
	point -= pos;
    // create a new surface

    Canvas newrect =  createCompatible(Point (w, h));

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
    SDL_SetColorKey( data(), SDL_SRCCOLORKEY|SDL_RLEACCEL, colorkey.toInt(data()) );
	substituteData (SDL_DisplayFormat(data()));//!!! What does this line do?
}
//--------------------------------------------------------------------------------------------------
void Canvas::setTotalTransparency (Uint8 degree)
{
	SDL_SetAlpha(data(), SDL_SRCALPHA, degree);
}
//--------------------------------------------------------------------------------------------------
void Canvas::setTransparentPixel (Point point)
{
	point -= pos;
    setPixel (point, data()->format->colorkey);
}
//--------------------------------------------------------------------------------------------------
bool Canvas::isTransparentPixel (Point point) const
{
	point -= pos;
    Uint32 pixelcolor = getPixel(point).Toint( this);
	//test whether pixels color == color of transparent pixels for that surface
	return (pixelcolor == data()->format->colorkey);
}
//--------------------------------------------------------------------------------------------------
Color Canvas::getTransparency() const
{
    Uint8 r, g, b, a;
    SDL_GetRGBA(data()->format->colorkey, data()->format, &r, &g, &b, &a);
    return Color (r, g, b, a);
}
//--------------------------------------------------------------------------------------------------
Canvas* Canvas::getScreenCanvas (Point size)
{
	assert(Screen == 0);//Run once
	if (Screen == 0)
		Screen = new Canvas (SDL_SetVideoMode (size.x, size.y, 0, 0));
	return Screen;
}
//--------------------------------------------------------------------------------------------------

void Canvas::ortogonalToIsometric()
{
    Canvas new_surf = createCompatible( Point (getWidth() * 4 - 2, getHeight() * 2));
	/* Set transparent background on the new tile */
	new_surf.fillRect(Rect (0, 0, new_surf.getWidth(), new_surf.getHeight()), Color (255, 0, 255));
	new_surf.setTransparency (Color (255, 0, 255));
	
	//_transformCoef = sqrt( 5 - 4 / getWidth() + 1 / (getWidth() * getWidth()));
	/* Set filling */
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
                tmp_vec.push_back( getPixel(Point (i - dj - 1, dj   )));
                tmp_vec.push_back( getPixel(Point (i - dj    , dj    )));
                tmp_vec.push_back( getPixel(Point (i - dj - 1, dj + 1 )));
                tmp_vec.push_back( getPixel(Point (i - dj    , dj + 1 )));
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
    reinit (new_surf);
	pos = transform(pos) - getWidth() / 2;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Rect::Rect (const SDL_Rect& that):x(that.x), y(that.y), w(that.w), h(that.h){}
//--------------------------------------------------------------------------------------------------
Rect::operator SDL_Rect() const
{
    SDL_Rect tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.w = w;
    tmp.h = h;
    return tmp;
}
//--------------------------------------------------------------------------------------------------

void Canvas::saveToBmp(string filename)
{
	SDL_SaveBMP(this->getSurface (), filename.c_str ());
}

