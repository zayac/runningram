/*
 * File:   Effects_manager.h
 * Author: vasily
 *
 * Created on April 5, 2010, 18:16
 */

#include "Effects_manager.h"

#include "Canvas.h"
#include "Explosion.h"
#include "initparser.h"


class Effects_manager::Initialaiser : public Sectionp
{
public:
        string explosion;
public:

	Initialaiser (string name)
	: Sectionp (name, '=')
	{
                Add_param (new St_loader<string> ("explosion", &explosion));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
};


Effects_manager::Effects_manager() : parser(new Initialaiser ("[Effects]"))
{
    
}

Effects_manager::~Effects_manager()
{
    delete parser;
}

bool Effects_manager::Init()
{
//    return Ok();
}

void Effects_manager::exp_draw(Canvas* can, int x, int y, bool * b)
{
    exp.Draw(can, x, y, b);
}

Serializator* Effects_manager::Get_parser()
{
	return parser;
}