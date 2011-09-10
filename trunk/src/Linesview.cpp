/*
 * Linesview.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: necto
 */

#include "Linesview.h"



//--------------------------------------------------------------------------------------------------
void LinesView::init (const Rect& brd)
{
	assert(ok());
	borders = brd;
}
//--------------------------------------------------------------------------------------------------
void LinesView::pushString (Stringc what)
{
	assert(ok());
	data.push_back (what);// reference, i receive an error, connected with font
	if (data.size () > max_lines)
		data.erase (data.begin());
}
//--------------------------------------------------------------------------------------------------
void LinesView::pushString (const string& str)
{
	pushString (Stringc (str, fontDefault));
}
//--------------------------------------------------------------------------------------------------
void LinesView::changeLastString (Stringc what)
{
	assert(ok());
	data.pop_back();
	data.push_back (Stringc(what));
}
//--------------------------------------------------------------------------------------------------
void LinesView::changeLastString (const string& str)
{
	assert(ok());
	changeLastString (Stringc (str, fontDefault));
}
//--------------------------------------------------------------------------------------------------
void LinesView::flush()
{
	assert(ok());
	pushString ("");
}
//--------------------------------------------------------------------------------------------------
void LinesView::freshLine()
{
	assert(ok());
	if (data.back() != "")
		pushString ("");
}
//--------------------------------------------------------------------------------------------------
int LinesView::Draw_tolerable_line (Canvas* screen, const Stringc& text, int offset, Rect brd) const
{
	assert(ok());
	Stringc buf = text.getBorderedSubstring (brd.w, offset);
	if (buf.draw (screen, &brd))
		return buf.length();
	return false;
}
//--------------------------------------------------------------------------------------------------
int LinesView::drawText (Canvas* screen, const Stringc& text, int start_offset) const
{
	assert(ok());
	int result_height = 0;
	int total_symbols = text.length();
	int symbols_showed = 0;

	Rect brd = borders;
	brd.cutTop (start_offset);

	int fheight = fontDefault.height ();

	while (symbols_showed < total_symbols)
	{
		int num_sym_in_line = Draw_tolerable_line (screen, text, symbols_showed, brd);

		if (num_sym_in_line == 0) return false;

		symbols_showed += num_sym_in_line;
		result_height += fheight;

		if (brd.h < fheight) break;
		brd.cutTop (fheight);
	}
    return result_height;
}
//--------------------------------------------------------------------------------------------------
void LinesView::draw (Canvas* c) const
{
	assert(ok());
	int start_y = 0;
	list <Stringc>::const_iterator start = data.begin();
	int height = 0;
	bool lesser = true;

	for (list <Stringc>::const_reverse_iterator i = data.rbegin(); i != data.rend(); ++i)
	{
		if (i->empty()) continue;
		height += i->getFullHeight (borders.w);
		if (height > borders.h)
		{
			start_y = height - borders.h;
			start = (++i).base();
			lesser = false;
			break;
		}
	}
	if (lesser) start_y = height - borders.h;//snuggle bottom

	c->fillRect (borders, fontDefault.bg);
	int cur_height = 0;
	int total_height = -start_y;
	for (list <Stringc>::const_iterator i = start; i != data.end(); ++i)
	{
		if (i->empty()) continue;
		cur_height = drawText (c, *i, total_height);
		if (cur_height == 0 && i->length() > 0) return;//!!! Error
		total_height += cur_height;
	}

}
//--------------------------------------------------------------------------------------------------
bool LinesView::ok() const
{
#ifdef FULL_DEBUG
	for (list<Stringc>::const_iterator i = data.begin(); i != data.end(); ++i)
		if (!i->Ok ()) return false;
#endif
	return fontDefault.ok();
}
//--------------------------------------------------------------------------------------------------
