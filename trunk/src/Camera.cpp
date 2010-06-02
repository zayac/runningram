/* 
 * File:   Camera.cpp
 * Author: necto
 * 
 * Created on January 8, 2010, 2:57 AM
 */

#include "Camera.h"
#include "Car.h"

Camera::Camera ():target(0)
{
}
//--------------------------------------------------------------------------------------------------
Camera::~Camera ()
{
}
//--------------------------------------------------------------------------------------------------
Point Camera::getPos() const
{
	return pos;
}
//--------------------------------------------------------------------------------------------------
void Camera::actions()
{
	if (!target) return;
	if (target->dead ())
	{
		target = 0;
		return;
	}

	pos += (Canvas::transform (target->getPos().to<int>()) - Point (300, 300) - pos)/50;
}
//--------------------------------------------------------------------------------------------------