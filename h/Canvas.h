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
#include "initparser.h"
#include "Rect.h"
#include <string>
#include <vector>
class Canvas;
class SDL_Surface;

const float SINPI4 = 0.707106781;

using namespace std;

class Color// :public SDL_Color
{
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 unused;

	class Initialiser;

public:
	Color (Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 255)
	 :r(red), g(green), b(blue), unused(alpha) {}
	Uint32 Toint (const Canvas* screen) const;

	static Color average(vector<Color>* colors);
	#ifdef _SDL_H
	operator SDL_Color() const;
	Color (const SDL_Color& src);

	Uint32 Toint (const SDL_Surface* screen) const;
	#endif
};

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
	inline Canvas cropRect (Rect tgt, bool remember_pos = false)
			{return cropRect (Point (tgt.x, tgt.y), tgt.w, tgt.h, remember_pos);}
	void rotate (double angle);
	void rotate90() { rotate(90); }
	void rotate180() { rotate(180); };
	void rotate270() { rotate(270);}
	void flipHorizontal();
	void flipVertical();
	void setTransparency (Color colorkey);
	Color getTransparency() const;
	void zoom (double zoomx, double zoomy); // percentage to zoom in
	void line (Point start, Point finish, Color c);
	inline Color getPixel (Point point)  const;
	void setPixel (Point point, Color color);
	bool isTransparentPixel (Point point) const;
	void setTransparentPixel (Point point);
    void ortogonalToIsometric();
	void saveToBmp( string filename);

	inline static Point transform(Point old) { return Point (SINPI4 * (old.x - old.y), SINPI4/2 * (old.x + old.y)); }
	inline static Vector2f transform(Vector2f old) { return Vector2f (SINPI4 * (old.x - old.y), SINPI4/2 * (old.x + old.y)); }

	void setPos (Point npos) {pos = npos;}
	Point getPos () const   {return pos;} 

	void draw (Canvas* buffer, Point pos) const;
	void fillRect (Rect r, Color col);
	void fill (Color col);

	void copy (Canvas from, Rect src_brd = Rect (0, 0, 0, 0), Point to = Point());
	inline void copy (Canvas from, Point src_from, Rect dst_brd = Rect (0, 0, 0, 0))
	{
		copy (from, Rect(src_from.x, src_from.y, dst_brd.w, dst_brd.h), Point (dst_brd.x, dst_brd.y));
	}
	void update();
	
	Rect getClipRect() const;
	int getWidth() const;
	int getHeight() const;
	SDL_Surface* getSurface() { return data(); }
	//void getSurfaceInformation();

	inline bool valid() {return data() != 0;}
	//void setSurface (SDL_Surface* surf) {data = surf};
	Canvas createCompatible (Point size = Point()) const;

	bool Ok() const;

	static Canvas* getScreenCanvas (Point size);//Run once!!!
};


#ifdef INITPARSER_H_INCLUDED
// <editor-fold defaultstate="collapsed" desc="From file color initialaiser">
class Color::Initialiser : public Sectionp
{
public:
	int r, g, b, unused;
	Color* object;
protected:
	virtual bool After_read (ifstream &file)
	{
		*object = Color (r, g, b, unused);
		return true;
	}
public:

	Initialiser (string name, Color* what)
	: Sectionp (name, '='), object (what), r (what->r), g (what->g), b (what->b), unused (what->unused)
	{
		Add_param (new St_loader<int > ("red", &r));
		Add_param (new St_loader<int > ("green", &g));
		Add_param (new St_loader<int > ("blue", &b));
		Add_param (new St_loader<int > ("alpha", &unused));
	}

	virtual ~Initialiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>
#endif //INITPARSER_H_INCLUDED

#endif	/* _CANVAS_H */

