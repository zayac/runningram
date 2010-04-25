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
    delete boom;
}

Effects_manager::Effects_manager(const Effects_manager& any)
{
    
}

bool Effects_manager::Init()
{
    boom = new Sprite("textures/image2_alpha.png", 33, 500, true);
//    return Ok();
}

void Effects_manager::exp_draw(Canvas* can)
{
    i = exp.begin();
    while( i != exp.end())
    {
        if( (**i).Draw(can ))
        {
            delete (*i);
            i = exp.erase(i);
        }
        else ++i;
    }
}

Serializator* Effects_manager::Get_parser()
{
	return parser;
}

void Effects_manager::Create_explosion (Vector2f pos, float size)
{
    Explosion* a = new Explosion;
    
    a->Set_sprite( boom);
    a->Set_position( pos.To<int>());
    exp.push_back(a);


        /* hello stl
        int a;
        list <int> li;
        list <int>::iterator it;
        for(a = 0; a<10; a++) li.push_back(a);
        li.erase(li.begin(), li.end());
        for(it = li.begin(); it != li.end(); ++it) printf("%d\n", *it);

*/

}