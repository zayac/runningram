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
#include "Vec.h"

class Canvas;

class Color :public SDL_Color
{
public:
	Color (Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 255)
	 {r =red; g = green; b = blue; unused = alpha; }
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

	void Draw (SDL_Surface* screen, Color c) const;

	void Cut_left (int);
	void Cut_top (int);
};

class Canvas :public SDL_Surface
{

	bool May_not_draw (Point& start, Point& finish) const;
	bool May_draw (Point& start, Point& finish) const;
	bool In_brd (Point& start, Point& finish, Point& delta) const;

public:
	Canvas();
//	virtual ~Canvas();

	void Line (Point start, Point finish, Color c);
	inline void Set_pixel (const Point& p, const Uint32& c) {((Uint32*)pixels)[p.x + p.y*w] = c;}
	inline void Set_pixel (const int &x, const int &y, const Uint32& c) {((Uint32*)pixels)[x + y*w] = c;}
	void Fill_rect (Rect r, Color col);
	inline void Fill (Color col) {Fill_rect (Rect (0, 0, w, h), col);}

	void Copy (Canvas* from, Rect src_brd = Rect (0, 0, 0, 0), Point to = Point());
	void Update();

	Canvas* Create_compatible (Point size = Point()) const;

	bool Ok() const;

};


#endif	/* _CANVAS_H */

