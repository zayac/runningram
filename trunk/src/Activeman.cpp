/* 
 * File:   Activeman.cpp
 * Author: necto
 * 
 * Created on January 12, 2010, 12:32 AM
 */

#include "Activeman.h"
#include "Car.h"
//
//Activeman::Activeman () { }
//
//Activeman::Activeman (const Activeman& orig) { }
//
//Activeman::~Activeman () { }

//--------------------------------------------------------------------------------------------------
void Activeman::Activate (double dt)
{
	for (iterator i = begin(); i != end(); ++i)
		(**i).Actions (dt);
}
//--------------------------------------------------------------------------------------------------
void Activeman::Draw (Canvas* c)
{
	for (iterator i = begin(); i != end(); ++i)
		(**i).Draw (c);
}
//--------------------------------------------------------------------------------------------------
bool Activeman::Ok() const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if (*i == 0 || !(**i).Ok()) return false;
	return true;
}
//--------------------------------------------------------------------------------------------------