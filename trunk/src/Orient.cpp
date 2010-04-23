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
	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
Orient::Orient (float angle, bool update) :ang (In_range (angle)), updated (update), dir (1, 0)
{
	if (update) Update();
	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
Orient::Orient (Vector2f direction, bool normed):ang (In_range (atan2(direction.y, direction.x))), dir (1, 0)
{
	if (!normed) Update ();
	else
	{
		dir = direction;
		updated = true;
	}
	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
Orient::Orient (const Orient& orig):ang (In_range (orig.ang)), dir (orig.dir), updated (orig.updated) { }
//--------------------------------------------------------------------------------------------------
Orient::~Orient () { }
//--------------------------------------------------------------------------------------------------
void Orient::Set_angle (float a)
{
	assert(Ok());
	ang = In_range (a);
	updated = false;
}
//--------------------------------------------------------------------------------------------------
void Orient::Update()
{
	assert(Ok());
	dir.x = cos (ang);
	dir.y = sin (ang);
	updated = true;
}
//--------------------------------------------------------------------------------------------------
float Orient::Get_angle() const
{
	assert(Ok());
	return ang;
}
//--------------------------------------------------------------------------------------------------
Vector2f Orient::Get_dir() const
{
	assert(Ok());
	if (updated) return dir;
	return Vector2f (sin (ang), cos (ang));
}
//--------------------------------------------------------------------------------------------------
Vector2f Orient::Get_dir()
{
	assert(Ok());
	if (!updated) Update();
	return dir;
}
//--------------------------------------------------------------------------------------------------
Orient Orient::operator -() const
{
	assert(Ok());
	Orient ret;
	ret.ang = - ang;
	if (updated) ret.dir.y = - dir.y;
	ret.updated = updated;
	return ret;
}
//--------------------------------------------------------------------------------------------------
Orient Orient::operator - (const Orient& that) const
{
	assert(Ok());
	Orient ret;
	ret.ang = In_range (ang - that.ang);
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
	assert(Ok());
	Orient ret;
	ret.ang = In_range (ang + that.ang);
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
	assert(Ok());
	*this = operator - (that);
	return *this;
}
//--------------------------------------------------------------------------------------------------
Orient& Orient::operator += (const Orient& that)
{
	assert(Ok());
	*this = operator + (that);
	
	if (!Ok())
	{
		bool gagaga = ( !updated || (1 - nearnull < dir.Lensq() && dir.Lensq() < nearnull + 1));
		bool re = !gagaga;
	}
	return *this;
}
//--------------------------------------------------------------------------------------------------
Vector2f Orient::Rotate (Vector2f what)
{
    if (!updated) Update();
    Vector2f ret;
    ret.x = what.x*dir.x - what.y*dir.y;
    ret.y = what.x*dir.y + what.y*dir.x;
    return ret;
}
//--------------------------------------------------------------------------------------------------
bool Orient::Ok() const
{
	return -PI <= ang && ang <= PI && (updated == true || updated == false) &&
		   ( !updated || (1 - nearnull < dir.Lensq() && dir.Lensq() < nearnull + 1));
}
//--------------------------------------------------------------------------------------------------