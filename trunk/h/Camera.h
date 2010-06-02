/* 
 * File:   Camera.h
 * Author: necto
 *
 * Created on January 8, 2010, 2:57 AM
 */

#ifndef _CAMERA_H
#define	_CAMERA_H

#include "Vec.h"

class Active;

class Camera
{
	Point pos;
	Active* target;

public:
	Camera();
	virtual ~Camera();

	Point getPos() const;
	bool hasTarget() const {return target != 0;}

	void setTarget (Active* a) {target = a;}

	void actions();

	bool ok() const {return true;}

};

#endif	/* _CAMERA_H */

