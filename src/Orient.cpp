/* 
 * File:   Orient.cpp
 * Author: necto
 * 
 * Created on January 10, 2010, 4:18 PM
 */
#include <math.h>
#include "Orient.h"


Orient::Orient () :ang(0), dir (1, 0), updated (true)
{
	assert(ok());
}
//--------------------------------------------------------------------------------------------------
Orient::Orient (float angle, bool _update) :ang (inRange (angle)), updated (_update), dir (1, 0)
{
	if (_update) update();
	assert(ok());
}
//--------------------------------------------------------------------------------------------------
Orient::Orient (Vector2f direction, bool normed) :ang (inRange (atan2(direction.y, direction.x))), dir (1, 0)
{
	if (!normed) update ();
	else
	{
		dir = direction;
		updated = true;
	}
	assert(ok());
}
//--------------------------------------------------------------------------------------------------
Orient::Orient (const Orient& orig):ang (inRange (orig.ang)), dir (orig.dir), updated (orig.updated) { }
//--------------------------------------------------------------------------------------------------
Orient::~Orient () { }
//--------------------------------------------------------------------------------------------------
void Orient::setAngle (float a)
{
	assert(ok());
	ang = inRange (a);
	updated = false;
}
//--------------------------------------------------------------------------------------------------
void Orient::update()
{
	assert(ok());
	dir.x = cos (ang);
	dir.y = sin (ang);
	updated = true;
}
//--------------------------------------------------------------------------------------------------
float Orient::getAngle() const
{
	assert(ok());
	return ang;
}
//--------------------------------------------------------------------------------------------------
Vector2f Orient::getDir() const
{
	assert(ok());
	if (updated) return dir;
	return Vector2f (sin (ang), cos (ang));
}
//--------------------------------------------------------------------------------------------------
Vector2f Orient::getDir()
{
	assert(ok());
	if (!updated) update();
	return dir;
}
//--------------------------------------------------------------------------------------------------
Orient Orient::operator -() const
{
	assert(ok());
	Orient ret;
	ret.ang = - ang;
	if (updated) ret.dir.y = - dir.y;
	ret.updated = updated;
	return ret;
}
//--------------------------------------------------------------------------------------------------
Orient Orient::operator - (const Orient& that) const
{
	assert(ok());
	Orient ret;
	ret.ang = inRange (ang - that.ang);
	ret.updated = updated && that.updated;
	if (ret.updated)
	{
		ret.dir.x = dir.x*that.dir.x + dir.y*that.dir.y;
		ret.dir.y = dir.y*that.dir.x - dir.x*that.dir.y;
	}
	return ret;
}
//--------------------------------------------------------------------------------------------------
Orient Orient::operator + (const Orient& that) const
{
	assert(ok());
	Orient ret;
	ret.ang = inRange (ang + that.ang);
	ret.updated = updated && that.updated;
	if (ret.updated)
	{
		ret.dir.x = dir.x*that.dir.x - dir.y*that.dir.y;
		ret.dir.y = dir.y*that.dir.x + dir.x*that.dir.y;
	}
	return ret;
}
//--------------------------------------------------------------------------------------------------
Orient& Orient::operator -= (const Orient& that)
{
	assert(ok());
	*this = operator - (that);
	return *this;
}
//--------------------------------------------------------------------------------------------------
Orient& Orient::operator += (const Orient& that)
{
	assert(ok());
	*this = operator + (that);
	
	if (!ok())
	{
		bool gagaga = ( !updated || (1 - nearnull < dir.lenSq() && dir.lenSq() < nearnull + 1));
		bool re = !gagaga;
	}
	return *this;
}
//--------------------------------------------------------------------------------------------------
Vector2f Orient::rotate (Vector2f what)
{
    if (!updated) update();
    Vector2f ret;
    ret.x = what.x*dir.x - what.y*dir.y;
    ret.y = what.x*dir.y + what.y*dir.x;
    return ret;
}
//--------------------------------------------------------------------------------------------------
bool Orient::ok() const
{
	return -PI <= ang && ang <= PI && (updated == true || updated == false) &&
		   ( !updated || (1 - nearnull < dir.lenSq() && dir.lenSq() < nearnull + 1));
}
//--------------------------------------------------------------------------------------------------