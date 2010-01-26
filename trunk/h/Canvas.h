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
#include "UniId.h"

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

//	bool May_not_draw (Point& start, Point& finish) const;
//	bool May_draw (Point& start, Point& finish) const;
//	bool In_brd (Point& start, Point& finish, Point& delta) const;
	Canvas (SDL_Surface* d);

	friend class Rect;
	friend class Color;
	friend class Fontc;

protected:
	virtual void Delete_data();
public:
	Canvas();
	Canvas (const Canvas& orig);
//	virtual ~Canvas();

	void Line (Point start, Point finish, Color c);
	inline void Set_pixel (const Point& p, const Uint32& c) {((Uint32*)data->pixels)[p.x + p.y*data->w] = c;}
	inline void Set_pixel (const int &x, const int &y, const Uint32& c) {((Uint32*)data->pixels)[x + y*data->w] = c;}
	void Fill_rect (Rect r, Color col);
	inline void Fill (Color col) {Fill_rect (Rect (0, 0, data->w, data->h), col);}

	void Copy (Canvas from, Rect src_brd = Rect (0, 0, 0, 0), Point to = Point());
	void Update();

	inline Rect Get_clip_rect() const {return data->clip_rect;}

	Canvas Create_compatible (Point size = Point()) const;
	bool Get_from_screen (Point size);//Run once!!!

	bool Ok() const;

};


#endif	/* _CANVAS_H */

