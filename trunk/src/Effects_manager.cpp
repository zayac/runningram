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
#include "Vec.h"
#include "Sprite.h"


class Effects_manager::Initialaiser : public Sectionp
{
public:
        string exp_file;
        int exp_frames;
        int exp_speed;
public:

	Initialaiser (string name)
	: Sectionp (name, '=')
	{
                Add_param (new St_loader<string> ("explosion file", &exp_file));
                Add_param (new St_loader<int> ("explosion frames", &exp_frames));
                Add_param (new St_loader<int> ("explosion speed", &exp_speed));
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
    delete boom;
}

Effects_manager::Effects_manager(const Effects_manager& any)
{
    
}

bool Effects_manager::Init()
{
    boom = new Sprite(parser->exp_file.c_str(), parser->exp_frames, parser->exp_speed, true);
//    return Ok();
}

void Effects_manager::exp_draw(Canvas* can)
{
    for(i = exp.begin(); i != exp.end(); ++i)
    {
        (**i).Draw(can );
        (**i).Animate();
    }
    
}

Serializator* Effects_manager::Get_parser()
{
	return parser;
}

void Effects_manager::Create_explosion (Vector2f pos, float size)
{
    Explosion* a = new Explosion;

    a->Set_sprite(boom);
    a->Set_position(pos.To<int>());
    exp.push_back(a);
    
}

void Effects_manager::exp_clean()
{
    i = exp.begin();
    while( i != exp.end())
    {
        if( !(**i).Get_run())
        {
            delete (*i);
            i = exp.erase(i);
        }
        else ++i;
    }
}