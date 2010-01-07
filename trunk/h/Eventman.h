/* 
 * File:   Eventman.h
 * Author: necto
 *
 * Created on January 7, 2010, 10:27 PM
 */

#ifndef _EVENTMAN_H
#define	_EVENTMAN_H

class Eventman
{
	bool stop;
public:
	Eventman();
	virtual ~Eventman();
public:

	void Acts();
	bool Stopped() const;

	bool Ok() const;
};

#endif	/* _EVENTMAN_H */

