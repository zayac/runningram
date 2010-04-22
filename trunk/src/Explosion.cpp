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

Explosion::Explosion()
{
//    texture = Canvas("textures/explosion.png");
    spr = new Sprite("textures/image2_alpha.png", 33, 1000, true);
}

Explosion::Explosion(Sprite* spr_): spr(spr_) {}

Explosion::~Explosion()
{
    delete spr;
}

void Explosion::Draw(Canvas* c, int x, int y, bool * b)
{


/*
    static int i = 0;
        if (i <= max_frames) spr->draw(c, Point( x, y));

        if (i == 0) spr->start();
        
        if (i > (max_frames - 1) )
        {
            spr->stop();
            b = false;
            i = -1;
        }
        spr->animate();
        
        printf("%d , %d\n", spr->getFrame(), i);
        c->update();

        if (i < (max_frames + 1) ) i++;

 */
    static int max = spr->getMaxFrames(), frame;
    
    frame = spr->getFrame();

    
    if (frame == 0) spr->start();
    spr->draw(c,Point(x, y));
    c->update();
    spr->animate();
    if (spr->getFrame() == 0)
    {
        spr->stop();
        *b = false;
    }
}
