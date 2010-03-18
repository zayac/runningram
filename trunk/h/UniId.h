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

private:
	base* _table;
	T* _data;
protected:

	inline void Register (base* tab)
	{
		if (tab == 0) _table = new base;
		else _table = tab;
		_table->push_front (this);
		myentry = _table->begin ();
	}
	inline void Unregister()
	{
		_table->erase (myentry);
		myentry = _table->end();
	}
	inline void Destroy()
	{
		if (_data == 0) return;
		Unregister();
		if (_table->empty())
		{
			delete _table;
			Delete_data();
			_data = 0;
		}
	}

	virtual void Delete_data()
	{
		delete _data;
	}

private:
	inline void Init (T* d, base* tab)
	{
		if (d != 0) Register (tab);
		_data = d;
	}
protected:
	inline void Reinit (T* d, base* tab)
	{
		if (d != _data)
		{
			Destroy();
			Init (d, tab);
		}
	}
	inline void Reinit (UniId that)
	{
		if (that._data != _data)
		{
			Destroy();
			Init (that._data, that._table);
		}
	}
	void Substitute_data (T* nd)
	{
		Delete_data();
		for (typename base::iterator i = _table->begin(); i != _table->end(); ++i)
		{
			(**i)._data = nd;
		}
	}

	UniId (T* d, base* tab):_data (d)
	{
		Init (d, tab);
	}

	inline T* data() const
	{
		return _data;
	}
	inline base* table() const
	{
		return _table;
	}
public:
	UniId (const UniId& orig):_data (orig._data)
	{
		if (_data == 0) _table = 0;
		else Register (orig._table);
	}
	virtual ~UniId()
	{
		Destroy();
	}

	inline UniId& operator= (const UniId& orig)
	{
//		Reinit (orig.data, orig._table);
		Reinit (orig);
		return *this;
	}

	virtual bool Ok() const
	{
		if (_data == 0) return _table == 0;//uninitialaised
		return _table != 0 && _table->size() > 0 && (*myentry) == this;
	}

};

#endif	/* _UNIID_H */

