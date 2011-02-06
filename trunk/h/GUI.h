/* 
 * File:   Interface.h
 * Author: necto
 *
 * Created on February 6, 2011, 2:34 PM
 */

#include "Rect.h"

#ifndef INTERFACE_H
#define	INTERFACE_H

class Serializator;
class Battlefield;
class Graphic_subsystem;
class Activeman;

class GUI
{
	Rect minimap;

	class Initialaiser;
	Initialaiser* parser;

public:
	GUI();
	virtual ~GUI();

	Serializator* getParser();

	bool init();

	void drawMiniMap (const Battlefield* bf, const Activeman* cars, Graphic_subsystem* gs);
};

#endif	/* INTERFACE_H */

