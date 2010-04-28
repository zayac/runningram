/* 
 * File:   Explosion.cpp
 * Author: vasily
 * 
 * Created on April 7, 2010, 22:15
 */

#include "Explosion.h"
#include "Sprite.h"
#include "Canvas.h"
#include <unistd.h>
#include "Vec.h"
#include <SDL/SDL.h>

Explosion::Explosion()
{
    run = true;
    index = 0;
    last_anim = SDL_GetTicks ();
}

Explosion::~Explosion()
{
//    delete spr;
}

bool Explosion::Draw(Canvas* c)
{
//    static int max = spr->getMaxFrames();

    if(run)
    {
        spr->setFrame(index);
        spr->draw(c, pos);
        c->update();
    }

/*
    if (spr->getFrame() == 0) spr->start();
    spr->draw(c, pos);
    c->update();
    spr->animate();
    if (spr->getFrame() == 0)
    {
        spr->stop();
        return true;
    }
    return false;
*/

}

void Explosion::Set_position(Point pos_)
{
    pos = pos_;
}

void Explosion::Set_sprite(Sprite* spr_)
{
    spr = spr_;
}

bool Explosion::Get_run(void )
{
    return run;
}

void Explosion::Animate(void )
{
    static int MaxFrames = spr->getMaxFrames(), speed = spr->getSpeed();
    
    if(index == (MaxFrames - 1))
    {
        run = false;
        index = 0;
    }
    if(run)
    {
        if (SDL_GetTicks () - last_anim > speed)
        {
            index++;
            last_anim = SDL_GetTicks ();
        }
    }
}