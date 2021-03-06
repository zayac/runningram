/* 
 * File:   Sprite.cpp
 * Author: zayac
 * 
 * Created on January 24, 2010, 6:54 PM
 */

#include <SDL/SDL.h>
#include <sstream>
#include "Sprite.h"
#include "Exception.h"
using namespace std;

Sprite::Sprite()
:speed (0), run (false), loopToBeginning (true), index (0), indexIterator (0)
{
}

Sprite::~Sprite()
{
    sprites.clear();
}

void Sprite::init (Canvas* surface, int maxFrames, int animationSpeed)
{
    static int I = 0;
    if(surface == NULL || !surface->valid()) 
	{
		throw Exception ("failed to load sprite");
		size = Point();
        this->speed = 0;
    } else {
		size.x = surface->getWidth() / maxFrames;
		size.y = surface->getHeight();
        clog << "successfully loaded sprite " << ++I << endl; //

        for(int i = 0; i <  maxFrames; i++)
        {
            Canvas temp;
            temp = surface->cropRect (Rect (Point (i * size.x, 0), size));
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

Sprite::Sprite (const char* file, int frames, int speed, bool alpha) {
    Canvas* can = new Canvas(file, alpha);
    init(can, frames, speed);
}

Sprite::Sprite(Canvas* canvas, int frames, int speed) {
    init(canvas, frames, speed);
}


bool Sprite::isRunning() const {
   return run;
}

void Sprite::stop() {
    run = false;
    index = 0;
}

int Sprite::getFrame() const {
    return index;
}

int Sprite::getMaxFrames() const
{
    return maxFrames;
}

void Sprite::setFrame(int index_)
{
	assert(0 <= index_ && index_ < maxFrames);
    index = index_;
}

void Sprite::animate()
{
	if (run)
	{
		if (SDL_GetTicks () - lastAnimated > speed)
		{
			lastAnimated = SDL_GetTicks ();
			index += indexIterator;
			if (index >= maxFrames)
			{
				if (loopToBeginning)
				{
					indexIterator = 1;
					index = 0;
				}
				else
				{
					indexIterator = -1;
					index = maxFrames - 1;
				}
			}
			else if (index < 0)
			{
				indexIterator = 1;
				index = 0;
			}
		}
	}
}

void Sprite::draw (Canvas* buffer, Point point)
{
    sprites[index].draw (*buffer, point);
}

void Sprite::draw (Canvas* buffer, Point point, float progress)
{
	sprites[int (progress*maxFrames)].draw (*buffer, point);
}


void Sprite::start()
{
    run = true;
}

void Sprite::restart()
{
    if(run) {
        index = 0;
        lastAnimated = SDL_GetTicks();
    }
}

void Sprite::setSpeed (int i)
{
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
    {
       sprites[i].zoom(zoomx, zoomy);
    }
	size = sprites[0].getSize();
}

void Sprite::ortogonalToIsometric()
{
    for(int i = 0; i < maxFrames; i++)
        sprites[i].ortogonalToIsometric();

	size = sprites[0].getSize();
}

int Sprite::getWidth() const
{
    return size.x;
}

int Sprite::getHeight() const
{
    return size.y;
}

Point Sprite::getSize() const
{
    return size;
}

void Sprite::saveToBmp (string filename)
{
	std::stringstream ss;
    std::string str;
	for(int i = 0; i < maxFrames;i++)
	{
		ss<<i;
		ss>>str;
		sprites[i].saveToBmp (filename.insert(filename.length() - 4, str));
	}
}

SDL_Surface* Sprite::getSurface (int i)
{
	sprites[i].getSurface ();
}