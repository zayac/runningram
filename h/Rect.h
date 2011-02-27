/* 
 * File:   Rect.h
 * Author: necto
 *
 * Created on May 28, 2010, 4:49 PM
 */

#ifndef _RECT_H
#define	_RECT_H
#include "mstdint.h"
#include "Vec.h"

class Rect// :public SDL_Rect
{
public:
	Sint16 x, y;
	Uint16 w, h;

public:
	Rect ():x(), y(), w(), h(){}
	Rect (Point lup, Point size) :x (lup.x), y (lup.y), w (size.x), h (size.y) {}
	Rect (int x, int y, int w, int h);
	Rect (const Rect& that);
	Rect& operator = (const Rect& that);

	void cutLeft (int);
	void cutTop (int);
	inline void enlarge (Point p)
	{
		w += p.x;
		h += p.y;
	}

	inline Point getLUp() const {return Point (x, y);}
	inline Point getSize() const {return Point (w, h);}
	inline Point getRDown() const {return getLUp() + getSize();}
	inline Point getCenter() const {return getLUp() + getSize()/2;}
	inline int getLeft() const {return x;}
	inline int getUp() const {return y;}
	inline int getRight() const {return x + w;}
	inline int getDown() const {return y + h;}

	inline bool contain (Point p) const
	{
		return	x < p.x && p.x - x < w &&
				y < p.y && p.y - y < h ;
	}
	inline bool intersect (const Rect& r)
	{
		return	x < r.x + r.w && r.x < x + w &&
				y < r.y + r.h && r.y < y + w;
	}

	inline void move (Point by) {x += by.x; y += by.y;}

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

#endif	/* _RECT_H */

