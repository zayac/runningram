/* 
 * File:   Sprite.cpp
 * Author: zayac
 * 
 * Created on January 24, 2010, 6:54 PM
 */

#include <SDL/SDL.h>
#include "Sprite.h"
#include "Exeption.h"
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

Sprite::~Sprite()
{
    sprites.clear();
}

void Sprite::init(Canvas* surface, int maxFrames, int animationSpeed)
{
    if(surface == NULL || !surface->valid()) 
	{
		throw Exeption ("failed to load sprite");
        width = 0; height = 0;
        this->speed = 0;
    } else {
        width = surface->getWidth() / maxFrames;
        height = surface->getHeight();
        clog << "successfully loaded sprite" << endl;

        for(int i = 0; i <  maxFrames; i++)
        {
            Canvas temp = surface->createCompatible(Point (width, height));
            temp = surface->cropRect(Point (i * width, 0), width, height);
            sprites.push_back(temp);
        }
        this->speed = animationSpeed;
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

Sprite::Sprite (const char* file, int frames, int speed) {
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
    sprites[index].draw(buffer, point);
}

void Sprite::draw(Canvas* buffer, Point point, float progress)
{
	sprites[int (progress*maxFrames)].draw (buffer, point);
}


void Sprite::start() {
    run = true;
}

void Sprite::restart() {
    if(run) {
        index = 0;
        lastAnimated = SDL_GetTicks();
    }
}

void Sprite::setSpeed (int i) {
    speed = i;
}

int Sprite::getSpeed()
{
    return speed;
}

void Sprite::setPos (Point npos)
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].setPos (npos);
}

void Sprite::rotate (double angle)
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].rotate(angle);
}

void Sprite::rotate90()
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].rotate90();
}

void Sprite::rotate180()
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].rotate180();
}


void Sprite::rotate270()
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].rotate270();
}

void Sprite::flipHorizontal()
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].flipHorizontal();
}

void Sprite::flipVertical()
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].flipVertical();
}

void Sprite::setTransparency (Color colorkey)
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].setTransparency(colorkey);
}

Color Sprite::getTransparency()
{
    return sprites[index].getTransparency();
}

void Sprite::zoom(double zoomx, double zoomy)
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].zoom(zoomx, zoomy);

}

void Sprite::ortogonalToIsometric()
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].ortogonalToIsometric();

}