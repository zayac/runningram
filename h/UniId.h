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
	typename base::iterator myentry_;

private:
	base* table_;
	T* data_;
protected:

	inline void registerm (base* tab)
	{
		if (tab == 0) table_ = new base;
		else table_ = tab;
		table_->push_front (this);
		myentry_ = table_->begin ();
	}
	inline void unregister()
	{
		table_->erase (myentry_);
		myentry_ = table_->end();
	}
	inline void destroy()
	{
		if (data_ == 0) return;
		unregister();
		if (table_->empty())
		{
			delete table_;
			deleteData();
			data_ = 0;
		}
		table_ = 0;
		data_ = 0;
	}

	virtual void deleteData() = 0;//If this function tries to be called,
								   //you must call destroy() from destructor of your class

private:
	inline void init (T* d, base* tab)
	{
		if (d != 0) registerm (tab);
		data_ = d;
	}
protected:
	inline void reinit (T* d, base* tab)
	{
		if (d != data_)
		{
			destroy();
			init (d, tab);
		}
	}
	inline void reinit (const UniId& that)
	{
		if (that.data_ != data_)
		{
			destroy();
			init (that.data_, that.table_);
		}
	}
	void substituteData (T* nd)
	{
		deleteData();
		for (typename base::iterator i = table_->begin(); i != table_->end(); ++i)
		{
			(**i).data_ = nd;
		}
	}

	UniId (T* d, base* tab):data_ (d)
	{
		init (d, tab);
	}

	inline T* data() const
	{
		return data_;
	}
	inline base* table() const
	{
		return table_;
	}
public:
	UniId (const UniId& orig):data_ (orig.data_)
	{
		if (data_ == 0) table_ = 0;
		else registerm (orig.table_);
	}
	virtual ~UniId()
	{
		destroy();
	}

	inline UniId& operator= (const UniId& orig)
	{
		reinit (orig);
		return *this;
	}

	virtual bool ok() const
	{
		if (data_ == 0) return table_ == 0;//uninitialaised
		return table_ != 0 && table_->size() > 0 && (*myentry_) == this;
	}

};

#endif	/* _UNIID_H */

