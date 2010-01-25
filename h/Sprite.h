/* 
 * File:   Sprite.h
 * Author: zayac
 *
 * Created on January 24, 2010, 6:38 PM
 */

#ifndef _SPRITE_H
#define	_SPRITE_H

#define SPRITE_MAX(a,b)	((a > b) ? a : b)
#define SPRITE_MIN(a,b)	((a < b) ? a : b)

#define FLIP_HORIZONTAL 1
#define FLIP_VERTICAL   2

#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include "Vec.h"
using namespace std;

class Sprite
{
private:
    /* static helper functions */
    static SDL_Surface* getRect(Sprite &sprite, Point point, int width, int height);
    static bool rectCollide(Sprite &spriteA, Point pointA, Sprite &spriteB, Point pointB);
    static bool pixelCollide(Sprite &spriteA, Point pointA, Sprite &spriteB, Point pointB);
    static void rotate90(Sprite &sprite);
    static void rotate180(Sprite &sprite);
    static void rotate270(Sprite &sprite);
    static void flipHorizontal(Sprite &sprite);
    static void flipVertical(Sprite &sprite);
    static void rotate(Sprite &sprite, int dir); // helper function for rotate90(),rotate180(), and rotate270()
    static void flip(Sprite &sprite, int dir);  // helper function for flipHorizontal() and flipVeritcal()
    static void fade(Sprite &sprite, float fade); // fade from 0 to 100%
    static void reverseAnimation(Sprite &sprite);

    static void zoom(Sprite &sprite, float x);  // percentage to zoom in
    static void stretchX(Sprite &sprite, float x); // percentage to stretchheight
    static void stretchY(Sprite &sprite, float y);  // percentage to stretchwidth
    static void stretch(Sprite &sprite, float x,float y); // percentage to strech X and Y

    static Uint32 getPixel(SDL_Surface* sprite, Point point);
    static void setPixel(SDL_Surface* sprite, Point point, Uint32 color);
public:
    Sprite();
    ~Sprite();
    Sprite(string fileName, int maxFrames, int animationSpeed);
    Sprite(SDL_Surface* surface, int maxFrames,int animationSpeed);

    void draw(SDL_Surface* buffer, Point point, int w, int h);
    void draw(SDL_Surface* buffer, Point point);
    void setTransparency(Uint32 color);
    void setTransparency(int red, int green, int blue);
    void setSpeed(int animationSpeed);
    void start();
    void restart();
    void animate();
    void setLoopToBegin(bool loop);
    bool running();
    void stop();
    bool isSprite();
    int getFrame();
    int getFrameWidth();
    int getFrameHeight();
    int getWidth();
    int getHeight();
    int getSpriteWidth();
    int getSpriteHeight();
    bool equals(Sprite compare);
    SDL_Surface* getSurface();
    void setSurface(SDL_Surface* surface);
    void destroy();

    bool isTransparentPixel(Point point);
    void setTransparentPixel(Point point);
    Uint32 getPixel(Point point);
    void setPixel(Point point, Uint32 color);
    void setPixel(Point point, int red, int green, int blue);
    Uint8 getPixel8(Point point);
    void setPixel8(Point point, Uint8 color);
    Uint16 getPixel16(Point point);
    void setPixel16(Point point, Uint16 color);
    Uint32 getPixel32(Point point);
    void setPixel32(Point point, Uint32 color);

    SDL_Surface* getRect(Point point, int width, int height);
    bool rectCollide(Point point1, Sprite &spriteB, Point point2);
    bool pixelCollide(Point point1, Sprite &spriteB, Point point2);
    void rotate90();
    void rotate180();
    void rotate270();
    void flipHorizontal();
    void flipVertical();
    void fade(float fade); // fade from 0 to 100%
    void reverseAnimation();

    void zoom(float z);  // percentage to zoom in
    void stretchX(float x); // percentage to stretchheight
    void stretchY(float y);  // percentage to stretchwidth
    void stretch(float x,float y); // percentage to strech X and Y


    bool loaded;
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

    SDL_Surface* sprite;
};

#endif	/* _SPRITE_H */