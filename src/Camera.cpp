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
Point Camera::Get_pos() const
{
	return pos;
}
//--------------------------------------------------------------------------------------------------
void Camera::Actions()
{
	if (!target) return;
	if (target->Dead ())
	{
		target = 0;
		return;
	}

	pos += (Canvas::transform (target->Get_pos().To<int>()) - Point (300, 300) - pos)/50;
}
//--------------------------------------------------------------------------------------------------