/* 
 * File:   UniId.h
 * Author: necto
 *
 * Created on January 26, 2010, 12:52 PM
 */

#ifndef _UNIID_H
#define	_UNIID_H
#include <list>

using std::list;
template <class T>
class UniId;

template <class T>
class UniId
{
protected:
typedef list <UniId<T> *> base;

	typename base::iterator myentry;
	base* table;
	T* data;

	inline void Register (base* tab)
	{
		if (tab == 0) table = new base;
		else table = tab;
		table->push_front (this);
		myentry = table->begin ();
	}
	inline void Unregister()
	{
		table->erase (myentry);
		myentry = table->end();
	}
	inline void Destroy()
	{
		if (data == 0) return;
		Unregister();
		if (table->empty())
		{
			delete table;
			Delete_data();
			data = 0;
		}
	}

	virtual void Delete_data()
	{
		delete data;
	}

private:
	inline void Init (T* d, base* tab)
	{
		if (d != 0) Register (tab);
		data = d;
	}
protected:
	inline void Reinit (T* d, base* tab)
	{
		if (d != data)
		{
			Destroy();
			Init (d, tab);
		}
	}

	UniId (T* d, base* tab):data (d)
	{
		Init (d, tab);
	}
public:
	UniId (const UniId& orig):data (orig.data)
	{
		if (data == 0) table = 0;
		else Register (orig.table);
	}
	virtual ~UniId()
	{
		Destroy();
	}

	UniId& operator= (const UniId& orig)
	{
		Reinit (orig.data, orig.table);
		return *this;
	}

	virtual bool Ok() const
	{
		if (data == 0) return table == 0;//uninitialaised
		return table != 0 && table->size() > 0 && (*myentry) == this;
	}

};

#endif	/* _UNIID_H */

