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

	void Move_right();
	void Move_left();
	void Move_up();
	void Move_down();

	Point Get_pos() const;

	void Set_target (Active* a) {target = a;}

	void Actions();

	bool Ok() const {return true;}

};

#endif	/* _CAMERA_H */

