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
	Rect (int x, int y, int w, int h);
	Rect (const Rect& that);
	Rect& operator = (const Rect& that);

	void Cut_left (int);
	void Cut_top (int);

	inline Point Get_lup() const {return Point (x, y);}
	inline Point Get_size() const {return Point (w, h);}
	inline Point Get_rdown() const {return Get_lup() + Get_size();}

	inline bool Contain (Point p) const
	{
		return	x < p.x && p.x - x < w &&
				y < p.y && p.y - y < h ;
	}

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

#endif	/* _RECT_H */

