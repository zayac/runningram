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

class Serializator;

class Effects_manager
{
    class Initialaiser;
    Initialaiser* parser;
    Explosion exp;
public:
    Effects_manager();
    virtual ~Effects_manager();

	void Create_explosion (Vector2f pos, float size){}
    
    Serializator* Get_parser();
    bool Init();
    void exp_draw(Canvas*, int, int, bool *);
};


#endif	/* _EFFECTS_MANAGER_H */

