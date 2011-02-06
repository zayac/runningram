/* 
 * File:   Interface.cpp
 * Author: necto
 * 
 * Created on February 6, 2011, 2:34 PM
 */

#include "GUI.h"
#include "initparser.h"
#include "Battlefield.h"
#include "Activeman.h"
#include "Car.h"


// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class GUI::Initialaiser : public Sectionp
{
public:
	Point minimapR;
	Point minimapSize;
public:

	Initialaiser (string name)
	: Sectionp (name, '='), minimapR(), minimapSize (150, 150)
	{
		addParam (new St_loader<int > ("minimap x", &minimapR.x));
		addParam (new St_loader<int > ("minimap y", &minimapR.y));
		addParam (new St_loader<int > ("minimap width", &minimapSize.x));
		addParam (new St_loader<int > ("minimap height", &minimapSize.y));
	}

	virtual ~Initialaiser ()
	{
		deleteProps ();
	}
}; // </editor-fold>


GUI::GUI():parser(new Initialaiser("[GUI]"))
{
}
//--------------------------------------------------------------------------------------------------
GUI::~GUI()
{
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* GUI::getParser()
{
	return parser;
}
//--------------------------------------------------------------------------------------------------
bool GUI::init()
{
	minimap = Rect(parser->minimapR, parser->minimapSize);
	return true;
}
//--------------------------------------------------------------------------------------------------
void GUI::drawMiniMap (const Battlefield* bf, const Activeman* cars, Graphic_subsystem* gs)
{
	bf->drawMinimap (gs, minimap);

	Canvas* canv = gs->getScreen();
	Point savedPos = canv->getPos();
	canv->setPos(-minimap.getLUp());

	Point scale = (bf->getFullSize())|(minimap.getSize());
	for (Activeman::const_iterator i = cars->begin(); i != cars->end(); ++i)
	{
		canv->fillRect(Rect ((**i).getPos().to<int>()|scale, Point(50, 50)|scale), Color(100, 100, 250));
	}
	canv->setPos(savedPos);
}
//--------------------------------------------------------------------------------------------------