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
	char id_;
protected:
	bool important_;

protected:
	Transmitted (char _id, bool importa) :id_(_id), important_ (importa){}

public:
	virtual int exp (char* buffer, int size) const = 0;
	virtual int imp (char* buffer, int size) = 0;

	inline char id() const {return id_;}
	inline char important() const {return important_;}
};

#endif	/* _TRANSMITTED_H */

