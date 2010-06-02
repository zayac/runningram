/* 
 * File:   Activeman.h
 * Author: necto
 *
 * Created on January 12, 2010, 12:32 AM
 */

#ifndef _ACTIVEMAN_H
#define	_ACTIVEMAN_H
#include <list>
#include "Transmitted.h"

using std::list;

class Canvas;
class Active;
class Battlefield;

class Activeman :public list <Active*>, public Transmitted
{
	void kill(iterator start, iterator finish);

public:
	Activeman();
//	Activeman(const Activeman& orig);
//	virtual ~Activeman();

	void activate (float dt);
	void draw (Canvas*);
	void collisBrd (const Battlefield* bf);
	void processCollisions();
	bool deleteDeadalives();//returns true when one or more of Deadalives have found and deleted

	int exp (char* buffer, int size) const;
	int imp (char* buffer, int size);

	bool ok() const;
};

#endif	/* _ACTIVEMAN_H */

