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
    Sprite* boom;
    list <Explosion*> exp;
    list <Explosion*>::iterator i;
public:
    Effects_manager();
    Effects_manager(const Effects_manager&);
    virtual ~Effects_manager();

    void Create_explosion (Vector2f pos, float size);
    
    Serializator* Get_parser();
    bool Init();
    
    void exp_draw(Canvas* );
    void exp_clean();
};


#endif	/* _EFFECTS_MANAGER_H */

