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

	virtual void deleteData();

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
	void openFont (const char* fname, int height);

	void setBG (const Color& b) {bgcol = b;}
	void setFG (const Color& f) {col = f;}
	void setFQ (Font_quality nfq) {fq = nfq;}

	const Color& bg;
	const Color& fg;

	Point strSize (const char* str) const;
	inline int strHeight (const char* str) const {return strSize (str).y;}
	inline int strLen (const char* str) const    {return strSize (str).x;}
	int height() const;
	int approximateNumSymbols (int width) const;
	Canvas createLabel (const char* line, bool color_reverse = false, Rect* brd = 0) const;
	int drawLine (Canvas* screen, const char* line, Rect* brd, bool color_reverse = false) const;

	bool ok() const;
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
	virtual bool afterRead (ifstream &file)
	{
		object->openFont (fname.c_str(), height);
		if (quality == "good")
			object->setFQ (Fontc::fqGOOD);
		else if (quality == "transparent")
			object->setFQ (Fontc::fqTRANSPARENT);
		else
			object->setFQ (Fontc::fqBAD);
		return true;
	}
public:

	Initialiser (string name, Fontc* what)
	: Sectionp (name, '='), fname ("default.ttf"), height (16), quality ("bad"), object (what)
	{
		addParam (new St_loader<int > ("height", &height));
		addParam (new St_loader<string > ("file", &fname));
		addParam (new St_loader<string > ("quality", &quality));
	}

	virtual ~Initialiser ()
	{
		deleteProps ();
	}
}; // </editor-fold>
#endif

#endif	/* _FONTC_H */

