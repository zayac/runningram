/* 
 * File:   Canvas.h
 * Author: necto
 *
 * Created on January 10, 2010, 2:36 PM
 */

#ifndef _CANVAS_H
#define	_CANVAS_H
#include "mstdint.h"
#include "Vec.h"
#include "UniId.h"
#include <string>
#include <vector>
class Canvas;
class SDL_Surface;

using namespace std;

class Color// :public SDL_Color
{
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 unused;

public:
	Color (Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 255)
	 :r(red), g(green), b(blue), unused(alpha) {}
	Uint32 Toint (Canvas* screen) const;

        static Color average(vector<Color>* colors);
	#ifdef _SDL_H
	operator SDL_Color() const;
	Color (const SDL_Color& src);

	Uint32 Toint (SDL_Surface* screen) const;
	#endif
};

class Rect// :public SDL_Rect
{
public:
	Sint16 x, y;
	Uint16 w, h;

public:
	Rect ():x(), y(), w(), h(){}
	Rect (int x, int y, int w, int h);
	Rect (const Rect& that);
	Rect& operator = (const Rect& that);

	void Draw (Canvas* screen, Color c) const;

	void Cut_left (int);
	void Cut_top (int);

	inline Point Get_lup() const {return Point (x, y);}
	inline Point Get_size() const {return Point (w, h);}
	inline Point Get_rdown() const {return Get_lup() + Get_size();}

	inline void Move (Point by) {x += by.x; y += by.y;}

	#ifdef _SDL_H
	Rect (const SDL_Rect& that);
	operator SDL_Rect() const;
	#endif
};

#ifdef _SDL_H
inline SDL_Rect*		addSdl (Rect* p) {return (SDL_Rect*)p;}
inline const SDL_Rect*	addSdl (const Rect* p) {return (const SDL_Rect*)p;}
inline Rect*			cutSdl (SDL_Rect* p) {return (Rect*)p;}
inline const Rect*		cutSdl (const SDL_Rect* p) {return (const Rect*)p;}
#endif

class Canvas :public UniId<SDL_Surface>
{
	static Canvas* Screen;

	Canvas (SDL_Surface* d);

	Point pos;//permanent addition

	friend class Rect;
	friend class Color;
	friend class Fontc;

protected:
	virtual void deleteData();
public:
	Canvas();
	Canvas (const Canvas& orig);
	Canvas (const char* file, bool alpha = false);
	//virtual ~Canvas();

	/* helper methods */
	Canvas cropRect (Point point, int width, int height, bool remember_pos = false);
	void rotate (double angle);
	void rotate90() { rotate(90); }
	void rotate180() { rotate(180); };
	void rotate270() { rotate(270);}
	void flipHorizontal();
	void flipVertical();
	void setTransparency (Color colorkey);
	Color getTransparency();
	void zoom (double zoomx, double zoomy); // percentage to zoom in
	void line (Point start, Point finish, Color c);
	inline Color getPixel (Point point);
	void setPixel (Point point, Color color);
	bool isTransparentPixel (Point point);
	void setTransparentPixel (Point point);
    void ortogonalToIsometric();
	void saveToBmp( string filename);

	static Point transformPointToOrtogonal(Point old, Point size);

	void setPos (Point npos) {pos = npos;}
	Point getPos () const   {return pos;}

	void draw (Canvas* sprite, Point pos);
	void fillRect (Rect r, Color col);
	void fill (Color col);

	void copy (Canvas from, Rect src_brd = Rect (0, 0, 0, 0), Point to = Point());
	void update();
	
	Rect getClipRect() const;
	int getWidth();
	int getHeight();
	SDL_Surface* getSurface() { return data(); }
	//void getSurfaceInformation();

	inline bool valid() {return data() != 0;}
	//void setSurface (SDL_Surface* surf) {data = surf};
	Canvas createCompatible (Point size = Point()) const;

	bool Ok() const;

	static Canvas* getScreenCanvas (Point size);//Run once!!!
};

#endif	/* _CANVAS_H */

