/* 
 * File:   Camera.cpp
 * Author: necto
 * 
 * Created on January 8, 2010, 2:57 AM
 */

#include "Camera.h"
#include "Car.h"
#include "initparser.h"


// <editor-fold defaultstate="collapsed" desc="From file Initializer">


class Camera::Initializer :public Sectionp
{
public:
	int divider;

public:

	Initializer (const char* name)
	: Sectionp (name, '='), divider (40)
	{
		addParam (new St_loader<int> ("divider", &divider));
	}

	virtual ~Initializer ()
	{
		deleteProps ();
	}
}; // </editor-fold>


Camera::Camera ():target(0), divider (40), parser(0)
{
}
//--------------------------------------------------------------------------------------------------
Camera::~Camera ()
{
}
//--------------------------------------------------------------------------------------------------
Serializator* Camera::newParcer()
{
	parser = new Initializer ("[Camera]");
	return parser;
}
//--------------------------------------------------------------------------------------------------
bool Camera::init()
{
	if (!parser) return false;
	divider = parser->divider;
	return true;
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

	pos += (Canvas::transform (target->getPos().to<int>()) - Point (300, 300) - pos)/divider;
}
//--------------------------------------------------------------------------------------------------