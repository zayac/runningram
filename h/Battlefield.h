/* 
 * File:   Battlefield.h
 * Author: necto
 *
 * Created on January 8, 2010, 12:30 AM
 */

#ifndef _BATTLEFIELD_H
#define	_BATTLEFIELD_H

#include "Vec.h"

class Battlefield
{
	char* cells;
	Point size;

	void Clean_field (char fill_cell);

public:
	Battlefield();
	virtual ~Battlefield();

	bool Load_from_file (char* fname);
private:

};

#endif	/* _BATTLEFIELD_H */

