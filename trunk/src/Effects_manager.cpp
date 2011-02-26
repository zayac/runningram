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


// <editor-fold defaultstate="collapsed" desc="from file Initializer">

class Effects_manager::Initializer : public Sectionp
{
public:
	string exp_file;
	int exp_frames;
	int exp_speed;
	Effects_manager* efm;
protected:

	virtual bool afterRead (ifstream &file)
	{
		Sprite *sp = new Sprite (exp_file.c_str (), exp_frames, exp_speed, true);
		efm->expAdd (sp);
		return true;
	}
public:

	Initializer (string name, Effects_manager* effect)
	: Sectionp (name, '='), efm (effect)
	{
		addParam (new St_loader<string > ("sprite file", &exp_file));
		addParam (new St_loader<int> ("number of frames", &exp_frames));
		addParam (new St_loader<int> ("speed", &exp_speed));
	}

	virtual ~Initializer ()
	{
		deleteProps ();
	}
}; // </editor-fold>

Effects_manager::Effects_manager() : parser(new Initializer ("[Explosion]", this))
{
}

Effects_manager::~Effects_manager()
{
	delete parser;
	for (cur_expl = exp_sprites.begin (); cur_expl != exp_sprites.end (); ++cur_expl)
	{
		delete (*cur_expl);
	}
	exp_sprites.erase (exp_sprites.begin (), exp_sprites.end ());

}

bool Effects_manager::init()
{
	cur_expl = exp_sprites.begin();
	return true;
}

void Effects_manager::expDraw(Canvas* can)
{
	for (i = expls.begin (); i != expls.end (); ++i)
	{
		(**i).draw (can);
		(**i).animate ();
	}
}

Serializator* Effects_manager::getParser()
{
	return parser;
}

void Effects_manager::createExplosion (Vector2f pos, float size)
{
	++cur_expl;
	if (cur_expl == exp_sprites.end()) cur_expl = exp_sprites.begin();

	Explosion* expl = new Explosion (*cur_expl, pos.to<int>() - (*cur_expl)->getSize()/2);
	expls.push_back (expl);
}

void Effects_manager::expClean()
{
	i = expls.begin ();
	while (i != expls.end ())
	{
		if (!(**i).isRun ())
		{
			delete (*i);
			i = expls.erase (i);
		}
		else ++i;
	}
}

void Effects_manager::expAdd (Sprite* sp)
{
	exp_sprites.push_back (sp);
}