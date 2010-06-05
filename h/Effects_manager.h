/* 
 * File:   Effects_manager.h
 * Author: vasily
 *
 * Created on April 5, 2010, 18:16
 */

#ifndef _EFFECTS_MANAGER_H
#define	_EFFECTS_MANAGER_H

#include "Canvas.h"
#include "Explosion.h"
#include "Vec.h"

class Serializator;
using std::list;

class Effects_manager
{
	class Initialaiser;
	Initialaiser* parser;
	list <Sprite*> boom;
	int k; // number of sprites
	list <Sprite*>::iterator is;
	list <Explosion*> exp;
	list <Explosion*>::iterator i;
public:
	Effects_manager();
	virtual ~Effects_manager();

	void createExplosion(Vector2f pos, float size);

	Serializator* getParser();
	bool init();
	void expAdd(Sprite*);

	void expDraw(Canvas*);
	void expClean();
};


#endif	/* _EFFECTS_MANAGER_H */

