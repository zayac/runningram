/*
 * Linesview.h
 *
 *  Created on: Sep 10, 2011
 *      Author: necto
 */

#ifndef LINESVIEW_H_
#define LINESVIEW_H_

#include <list>
#include "Stringc.h"

using std::list;

class LinesView
{
	int max_lines;
	Fontc& fontDefault;
	Rect borders;
	list <Stringc> data;

	int drawText (Canvas* screen, const Stringc&, int start_offset) const;
	int Draw_tolerable_line (Canvas* screen, const Stringc&, int offset, Rect brd) const;

public:
	LinesView (Fontc& f, int max_lines_) :fontDefault(f), max_lines (max_lines_) {}

	void init (const Rect&);
	void pushString (Stringc);
	void pushString (const string& str);
	void changeLastString (Stringc);
	void changeLastString (const string& str);
	void flush();
	void freshLine();
	void draw (Canvas* c) const;

	bool ok() const;
};

#endif /* LINESVIEW_H_ */
