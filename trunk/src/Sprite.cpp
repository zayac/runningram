/* 
 * File:   Sprite.cpp
 * Author: zayac
 * 
 * Created on January 24, 2010, 6:54 PM
 */

#include "Sprite.h"
using namespace std;

Sprite::Sprite() {
    width = 0;
    height = 0;
    speed = 0;
    run = false;
    loopToBeginning = true;
    indexIterator = 0;
    index = 0;
}

void Sprite::init(Canvas* surface, int maxFrames, int animationSpeed)
{
    if(surface == NULL) {
        cout << "failed to load sprite" << endl;
        width = 0; height = 0;
        this->speed = 0;
    } else {
        width = surface->getWidth() / maxFrames;
        height = surface->getHeight();
        cout << "successfully loaded sprite" << endl;

        for(int i = 0; i <  surface->getWidth(); i++)
        {
            sprites.push_back(surface->getRect( Point (i * width, 0), width, height));
        }
        this->speed = speed;
    }
    this->maxFrames = maxFrames;
    if(maxFrames > 1) {
        run = true;
    } else {
        run = false;
    }
    lastAnimated = SDL_GetTicks();
    indexIterator = 1;
    index = 0;
    loopToBeginning = true;
}

Sprite::Sprite(char* file, int frames, int speed) {
    Canvas* can = new Canvas(file);
    init(can, frames, speed);
}

Sprite::Sprite(Canvas* canvas, int frames, int speed) {
    init(canvas, frames, speed);
}


bool Sprite::running() {
   return run;
}

void Sprite::stop() {
    run = false;
    index = 0;
}

int Sprite::getFrame() {
    return index;
}

void Sprite::animate() {
    if(run) {
        if(SDL_GetTicks() - lastAnimated > speed) {
            lastAnimated = SDL_GetTicks();
            index += indexIterator;
            if(index >= maxFrames) {
                if(loopToBeginning) {
                    indexIterator = 1;
                    index = 0;
                } else {
                    indexIterator = -1;
                    index = maxFrames - 1;
                }
            } else if(index < 0) {
                indexIterator = 1;
                index = 0;
            }
        }
    }
}

void Sprite::draw(Canvas* buffer, Point point) {
    sprites[index]->draw(buffer, point);
}

