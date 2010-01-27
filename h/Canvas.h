/* 
 * File:   Canvas.h
 * Author: necto
 *
 * Created on January 10, 2010, 2:36 PM
 */

#ifndef _CANVAS_H
#define	_CANVAS_H
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include "Vec.h"
#include "UniId.h"
#include <string>
class Canvas;

class Color :public SDL_Color
{
public:
	Color (Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 255)
	 {r =red; g = green; b = blue; unused = alpha; }
	Uint32 Toint (Canvas* screen) const;
	Uint32 Toint (SDL_Surface* screen) const;
};

class Rect :public SDL_Rect
{
public:
	Rect ():SDL_Rect(){}
	Rect (int x, int y, int w, int h);
	Rect (const Rect& that);
	Rect (const SDL_Rect& that);
	Rect& operator = (const Rect& that);

	void Draw (Canvas* screen, Color c) const;

	void Cut_left (int);
	void Cut_top (int);
};

class Canvas :public UniId<SDL_Surface>
{
	static base* screen_t;
	static SDL_Surface* screen_p;
        Canvas (SDL_Surface* d);

	friend class Rect;
	friend class Color;
	friend class Fontc;

protected:
	virtual void deleteData();
public:
	Canvas();
	Canvas (const Canvas& orig);
        Canvas(char* file);
//	virtual ~Canvas();

        /* helper methods */
        Canvas getRect(Point point, int width, int height);
        void rotate(double angle) { data = rotozoomSurface(data, angle, 1, 1); }
        void rotate90() { rotate(90); }
        void rotate180() { rotate(180); };
        void rotate270() { rotate(270);}
        void flipHorizontal()  { data = zoomSurface(data, 1, -1, SMOOTHING_ON); }
        void flipVertical() { data = zoomSurface(data, -1, 1, SMOOTHING_ON); }
        void setTransparency(Color colorkey);
        Color getTransparency();
        void zoom(double zoomx, double zoomy) { data = zoomSurface (data, zoomx, zoomy, SMOOTHING_ON);}  // percentage to zoom in
	void line (Point start, Point finish, Color c);
        inline Color getPixel(Point point);
        void setPixel(Point point, Color color);
        bool isTransparentPixel(Point point);
        void setTransparentPixel(Point point);

        void draw(Canvas* sprite, Point pos);
        void fillRect (Rect r, Color col);
	inline void fill (Color col) {fillRect (Rect (0, 0, data->w, data->h), col);}

        void copy (Canvas from, Rect src_brd = Rect (0, 0, 0, 0), Point to = Point());
	void update();
	
        inline Rect getClipRect() const {return data->clip_rect;}
        int getWidth() { return data->w; }
        int getHeight() { return data->h; }
        SDL_Surface* getSurface() { return data; }
        void setSurface(SDL_Surface* surf) {};
        Canvas createCompatible (Point size = Point()) const;
	bool getFromScreen (Point size);//Run once!!!
	bool Ok() const;
};


#endif	/* _CANVAS_H */

