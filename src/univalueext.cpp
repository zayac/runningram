/*
 * univalueext.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: necto
 */

#include "univalueext.h"


template<>
UniValue UniValue::by<Vector2f> (Vector2f v)
{
	CustomObject vect("vec");
	vect.set("x", v.x);
	vect.set("y", v.y);
	return vect.convert();
}
//--------------------------------------------------------------------------------------------------
template<>
UniValue UniValue::by<const Car*> (const Car* c)
{
	CustomObject car("car-properties");
	car.set ("pos", c->getPos());
	car.set ("vel", c->getVel(c->getPos()));
	return car.convert();
}
//--------------------------------------------------------------------------------------------------
