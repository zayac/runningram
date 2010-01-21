/* 
 * File:   Activeman.h
 * Author: necto
 *
 * Created on January 12, 2010, 12:32 AM
 */

#ifndef _ACTIVEMAN_H
#define	_ACTIVEMAN_H
#include <list>

using std::list;

class Canvas;
class Active;
class Battlefield;

class Activeman :public list <Active*>
{
public:
//	Activeman();
//	Activeman(const Activeman& orig);
//	virtual ~Activeman();

	void Activate (float dt);
	void Draw (Canvas*);
	void Collis_brd (const Battlefield* bf);

	bool Ok() const;
};

#endif	/* _ACTIVEMAN_H */

