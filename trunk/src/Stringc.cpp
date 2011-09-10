/*
 * Stringc.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: necto
 */

#include "Stringc.h"


//--------------------------------------------------------------------------------------------------
void Stringc::insert (int pos, int number, char c)
{
	assert(ok());
	text.insert(pos, number, c);
	if (lab_upd)
		updateLab();
}
//--------------------------------------------------------------------------------------------------
void Stringc::clear()
{
	assert(ok());
	text.clear();
	if (lab_upd)
		updateLab();
}
//--------------------------------------------------------------------------------------------------
void Stringc::erase (int pos, int number)
{
	assert(ok());
	text.erase (pos, number);
	if (lab_upd)
		updateLab();
}
//--------------------------------------------------------------------------------------------------
Stringc& Stringc::operator = (const string& str)
{
	assert(ok());
	text = str;
	if (lab_upd)
		updateLab();
	return *this;
}
//--------------------------------------------------------------------------------------------------
void Stringc::setFont (Fontc f)
{
	assert(ok());
	font = f;
	if (lab_upd)
		updateLab();
}
//--------------------------------------------------------------------------------------------------
int Stringc::draw (Canvas* screen, Rect* brd, int offset, bool color_reverse) const//!!!??? Offset may be is tooish
{
	assert(brd != 0);
	if (lab_upd && !color_reverse)//!!! Not debugged yet
	{
		int pos = font.strLen (substr(0, offset).c_str());
		screen->copy (lab, Point(pos, 0), Rect (brd->x, brd->y, brd->w - pos, brd->h));
		return min (brd->h, lab.getHeight());
	}
	else
		return font.drawLine (screen, c_str() + offset, brd, color_reverse);
}
//--------------------------------------------------------------------------------------------------
void Stringc::dontUpdateLab()
{
	assert(ok());
	lab_upd = false;
}
//--------------------------------------------------------------------------------------------------
void Stringc::updateLab()
{
	assert(ok());
	lab = font.createLabel (c_str(), false);
	lab_upd = true;
}
//--------------------------------------------------------------------------------------------------
int Stringc::getFullHeight (Uint16 max_width) const
{
	assert(ok());
	Point size = pSize();
	float nlines = float(size.x)/max_width;

	if (nlines - float(int(nlines)) > 0.01) nlines += 1;//for tail

	int num_lines = int (nlines);
	if (num_lines < 1) num_lines = 1;
	return size.y*num_lines;
}
//--------------------------------------------------------------------------------------------------
Stringc Stringc::getBorderedSubstring (int width, int offset) const
{
	int ret = font.approximateNumSymbols (width);
	if (0 >= ret || ret > length()) ret = length();
	if (ret > 500) ret = 500;

	char buf[512];
	strncpy (buf, c_str() + offset, ret);
	buf[ret] = 0;

	int w = font.strLen (buf);

	while (w > width)
	{
		float std_w = (float)(w)/ret;
		ret -= (w - width)/std_w + 0.5;
		ret -= ret%2; //for double char symbols
		buf[ret] = 0;//cut string
		w = font.strLen (buf);
	}
	return Stringc (buf, font);
}
//--------------------------------------------------------------------------------------------------
