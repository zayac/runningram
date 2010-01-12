/* 
 * File:   Camera.h
 * Author: necto
 *
 * Created on January 8, 2010, 2:57 AM
 */

#ifndef _CAMERA_H
#define	_CAMERA_H

#include "Vec.h"

class Camera
{
	Polong pos;

public:
	Camera();
	virtual ~Camera();

	void Move_right();
	void Move_left();
	void Move_up();
	void Move_down();

	Polong Get_pos() const;

	bool Ok() const {return true;}

};

#endif	/* _CAMERA_H */

