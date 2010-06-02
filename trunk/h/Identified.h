/* 
 * File:   Identified.h
 * Author: necto
 *
 * Created on April 22, 2010, 11:50 AM
 */

#ifndef _IDENTIFIED_H
#define	_IDENTIFIED_H

template <class Derived>
class Identified
{
	static int max_id;

protected:
	int id_;
public:
	inline Identified (int id = 0) :id_ (++max_id)
	{
		if (id)
		{
			id_ = id;
			if (max_id < id) max_id = id;
		}
	}

	inline int id() {return id_;}

};

template <class Derived>
int Identified<Derived>::max_id = 0;

#endif	/* _IDENTIFIED_H */

