/* 
 * File:   Fontc.h
 * Author: necto
 *
 * Created on February 14, 2010, 9:39 PM
 */

#ifndef _FONTC_H
#define	_FONTC_H

#include <assert.h>
#include "Canvas.h"
#include "UniId.h"

#ifndef _SDL_TTF_H
class TTF_Font;
#endif

#ifndef min
inline int min(int a, int b) {return a < b ? a : b;}
#endif

class Fontc :public UniId<TTF_Font>
{
	Color col;
	Color bgcol;

	virtual void Delete_data();

public:
	enum Font_quality
	{
		fqBAD,
		fqGOOD,
		fqTRANSPARENT
	};
	class Initialiser;
private:
	Font_quality fq;

public:
	Fontc ();
	Fontc (int height, const char* fname, Font_quality fq = fqBAD, Color fg = Color(), Color bg = Color());
	Fontc (const Fontc& orig);
	~Fontc();

	Fontc& operator= (const Fontc& orig);
	void Open_font (const char* fname, int height);

	void Set_bg (const Color& b) {bgcol = b;}
	void Set_fg (const Color& f) {col = f;}
	void Set_fq (Font_quality nfq) {fq = nfq;}

	const Color& bg;
	const Color& fg;

	Point Str_size (const char* str) const;
	inline int Str_height (const char* str) const {return Str_size (str).y;}
	inline int Str_len (const char* str) const    {return Str_size (str).x;}
	int Height() const;
	int Approximate_num_symbols (int width) const;
	Canvas Create_label (const char* line, bool color_reverse = false, Rect* brd = 0) const;
	int Draw_line (Canvas* screen, const char* line, Rect* brd, bool color_reverse = false) const;

	bool Ok() const;
};

void FontcCleanUp();

#ifdef INITPARSER_H_INCLUDED
// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class Fontc::Initialiser : public Sectionp
{
public:
	string fname;
	int height;
	string quality;

	Fontc* object;


protected:
	virtual bool After_read (ifstream &file)
	{
		object->Open_font (fname.c_str(), height);
		if (quality == "good")
			object->Set_fq (Fontc::fqGOOD);
		else if (quality == "transparent")
			object->Set_fq (Fontc::fqTRANSPARENT);
		else
			object->Set_fq (Fontc::fqBAD);
		return true;
	}
public:

	Initialiser (string name, Fontc* what)
	: Sectionp (name, '='), fname ("default.ttf"), height (16), quality ("bad"), object (what)
	{
		Add_param (new St_loader<int > ("height", &height));
		Add_param (new St_loader<string > ("file", &fname));
		Add_param (new St_loader<string > ("quality", &quality));
	}

	virtual ~Initialiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>
#endif

#endif	/* _FONTC_H */

