/* 
 * File:   Sprite.h
 * Author: zayac
 *
 * Created on January 24, 2010, 6:38 PM
 */

#ifndef _SPRITE_H
#define	_SPRITE_H

#include <iostream>
#include <string>
#include "Vec.h"
#include <vector>
#include "Canvas.h"
#include <SDL/SDL.h>
using namespace std;

class Sprite
{
private:

    bool run;
    int speed;
    int width;
    int height;
    int index;
    int indexIterator;
    bool loopToBeginning;  // if loop = true iterate through animations from 0 to N, then reset to 0
                // if loop = false iterate through animations from 0 to N, then from N to 0, and repeat
    int maxFrames;
    int lastAnimated;

    vector<Canvas> sprites;

    void init(Canvas* canvas, int maxFrames, int animationSpeed);
public:
    Sprite();
    ~Sprite();
    Sprite(char* fileName, int maxFrames, int animationSpeed);
    Sprite(Canvas* canvas, int maxFrames,int animationSpeed);

    bool running();
    void stop();
    int getFrame();
    void animate();
    void draw(Canvas* buffer, Point point);
};

#endif	/* _SPRITE_H */