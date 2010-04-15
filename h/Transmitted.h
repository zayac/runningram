/* 
 * File:   Transmitted.h
 * Author: necto
 *
 * Created on April 15, 2010, 12:51 PM
 */

#ifndef _TRANSMITTED_H
#define	_TRANSMITTED_H

const int Buffer_size = 1048576;

class Transmitted
{
	char id;

protected:
	bool important;
protected:
	Transmitted (char id_, bool important_) :id(id_), important (important_){}

public:
	virtual int Export (char* buffer, int size) const = 0;
	virtual int Import (char* buffer, int size) = 0;

	inline char Id() const {return id;}
	inline char Important() const {return important;}
};

#endif	/* _TRANSMITTED_H */

