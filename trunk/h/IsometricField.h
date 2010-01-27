/* 
 * File:   IsometricField.h
 * Author: zayac
 *
 * Created on January 25, 2010, 4:04 PM
 */

#ifndef _ISOMETRICFIELD_H
#define	_ISOMETRICFIELD_H
#include "Sprite.h"
#include <vector>
#include <string>
class IsometricField
{
public:
    IsometricField();
    IsometricField(Sprite src);
    IsometricField(string filename);
    virtual ~IsometricField();
    void draw(SDL_Surface* buffer, Point point);
private:
    std::vector<SDL_Surface*> frames; // frames
    int frame_number;
    int active_sprite;
    
    Uint32 getPixel(SDL_Surface* src, Point point);
    void setPixel(SDL_Surface* src, Point point, Uint32 color);
    SDL_Surface* createIsoProjection (SDL_Surface* src);

};

#endif	/* _IsometricField_H */

