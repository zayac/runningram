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

    void init (Canvas* canvas, int maxFrames, int animationSpeed);
public:
    Sprite();
    ~Sprite();
    Sprite (const char* fileName, int maxFrames, int animationSpeed, bool alpha = false);
    Sprite (Canvas* canvas, int maxFrames,int animationSpeed);

    void rotate (double angle);
    void rotate90();
    void rotate180();
    void rotate270();
    void flipHorizontal();
    void flipVertical();
    void setTransparency (Color colorkey);
    Color getTransparency();
    void zoom(double zoomx, double zoomy); // percentage to zoom in
	void saveToBmp(string filename);
	SDL_Surface* getSurface(int i);

    bool running();
    void stop();
    int getFrame();
    int getMaxFrames();
    int getWidth();
    int getHeight();
    void animate();
    void draw (Canvas* buffer, Point point);
    void draw (Canvas* buffer, Point point, float progress);
    void start();
    void restart();
    void setSpeed(int i);
    int getSpeed();
    void setFrame(int );

	void setPos (Point npos);

    void ortogonalToIsometric();

};

#endif	/* _SPRITE_H */