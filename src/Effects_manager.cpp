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
        Effects_manager* efm;
protected:
	virtual bool After_read (ifstream &file)
	{
            Sprite *sp = new Sprite(exp_file.c_str(), exp_frames, exp_speed, true);
            efm->exp_add(sp);
            return true;
	}
public:
	Initialaiser (string name, Effects_manager* effect)
	: Sectionp (name, '='), efm(effect)
	{
                Add_param (new St_loader<string> ("sprite file", &exp_file));
                Add_param (new St_loader<int> ("number of frames", &exp_frames));
                Add_param (new St_loader<int> ("speed", &exp_speed));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
};


Effects_manager::Effects_manager() : parser(new Initialaiser ("[Explosion]", this))
{
    
}

Effects_manager::~Effects_manager()
{
    delete parser;
    for(is = boom.begin(); is != boom.end(); ++is)
    {
        delete (*is);
    }
    boom.erase(boom.begin(), boom.end());

}

Effects_manager::Effects_manager(const Effects_manager& any)
{
    
}

bool Effects_manager::Init()
{
    /*
    boom.push_back(new Sprite(parser->exp_file.c_str(), parser->exp_frames, parser->exp_speed, true));
    boom.push_back(new Sprite("textures/image1.png", 31, 100, true));
    boom.push_back(new Sprite("textures/image2.png", 36, 100, true));
    boom.push_back(new Sprite("textures/image3.png", 36, 100, true));
    boom.push_back(new Sprite("textures/image4.png", 48, 100, true));
    
     */
//    return Ok();
    k = boom.size();
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
    static int x_os, y_os, j, b = 1;
    static Point pos_new;

    is = boom.begin();

    for(j = 1; j<b; ++j)
    {
        ++is;
    }
    b++;
    if(b>k) b = 1;

    x_os = (*is)->getWidth();
    y_os = (*is)->getHeight();

    a->Set_sprite( (*is) );
    pos_new = pos.To<int>();
    pos_new.x -= x_os/2;
    pos_new.y -= y_os/2;

    a->Set_position(pos_new);
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

void Effects_manager::exp_add(Sprite* sp)
{
    boom.push_back(sp);
}