/*
 * File:   Stringc.h
 * Author: Necto
 *
 * Created on September 10 (2011) at 22:42
 */

#ifndef _STRINGC_H_
#define	_STRINGC_H_

#include <string>
#include "Canvas.h"
#include "Fontc.h"

using std::string;

class Stringc
{
	string text;
	Fontc font;

	Canvas lab;
	bool lab_upd;

public:
	Stringc ():lab_upd (false){}
	Stringc (const Stringc& orig) :text (orig.text), font (orig.font), lab_upd (false) {}
	Stringc (const string& str, Fontc f) :text (str), font (f), lab_upd (false) {}

	void insert (int pos, int number, char c);
	void clear();
	void erase (int pos, int number);
	inline string::size_type length() const {return text.size();}
	inline string substr (int start, int number) const {return text.substr(start, number);}
	inline bool empty() const {return text.empty();}
	inline const char* c_str() const {return text.c_str();}
	inline bool operator != (const string& str) const {return text != str;}
	inline bool operator == (const string& str) const {return text == str;}

	Stringc& operator = (const string& str);
	void setFont (Fontc f);

	int draw (Canvas* screen, Rect* brd, int offset = 0, bool color_reverse = false) const;//!!!??? Offset may be is tooish

	void dontUpdateLab();
	void updateLab();

	inline Point pSize (int offset = 0) const {return font.strSize (text.c_str() + offset);}
	inline int height (int offset = 0) const {return pSize (offset).y;}
	inline int width (int offset = 0) const {return pSize (offset).x;}

	int getFullHeight (Uint16 max_width) const;

	Stringc getBorderedSubstring (int width, int offset = 0) const;
			//There are more problems with russian characters

	operator string& () {return text;}
	operator const string& () const {return text;}

	bool ok() const {return font.ok() && (!lab_upd || (lab.ok() && lab.valid()));}
};

#endif //_STRINGC_H_
