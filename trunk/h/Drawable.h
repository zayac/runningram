/* 
 * File:   Drawable.h
 * Author: necto
 *
 * Created on June 3, 2010, 12:06 AM
 */

#ifndef _DRAWABLE_H
#define	_DRAWABLE_H

class Drawable
{
	int height_;
protected:
	inline void setHeight (int nheigh) {height_ = nheigh;}

public:
	Drawable (int height = 0):height_(height){}
	virtual void draw (Canvas*) = 0;
	inline int getHeight() {return height_;}
};

#endif	/* _DRAWABLE_H */

