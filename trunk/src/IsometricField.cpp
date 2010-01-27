/* 
 * File:   IsometricField.cpp
 * Author: zayac
 * 
 * Created on January 25, 2010, 4:04 PM
 */

#include "IsometricField.h"
#include <math.h>
#include <SDL/SDL_rotozoom.h>


IsometricField::IsometricField()
{
}
IsometricField::IsometricField(Sprite src)
{
    frame_number = 0;
    for(int i = 0; i < src.maxFrames; i++)
    {
        SDL_Surface* temp_surf;
        SDL_Rect dstrect;
        dstrect.x = 0;
        dstrect.y = 0;
        // this blits the current frame from the sprite sheet
        SDL_Rect srcrect;
        srcrect.x = src.width*i;
        srcrect.y = 0;
        srcrect.w = src.width;
        srcrect.h = src.height;
        SDL_BlitSurface(src.getSurface(), &srcrect, temp_surf, &dstrect);
        frames.push_back(temp_surf);
        frame_number++;
        active_sprite = 0;
    }
}

IsometricField::IsometricField(string filename)
{
    SDL_Surface *temp = SDL_LoadBMP(filename.c_str());
    frames.push_back(createIsoProjection (SDL_DisplayFormat(temp)));
    frame_number = 1;
    active_sprite = 0;
}

void IsometricField::draw(SDL_Surface* buffer, Point point) {
    SDL_Rect dstrect;
    dstrect.x = point.x;
    dstrect.y = point.y;
    // this blits the current frame from the sprite sheet
    SDL_Rect animRect;
    animRect.x = 0;
    animRect.y = 0;
    animRect.w = frames[active_sprite]->w;
    animRect.h = frames[active_sprite]->h;
    SDL_BlitSurface(frames[active_sprite], &animRect, buffer, &dstrect);
}

SDL_Surface* IsometricField::createIsoProjection (SDL_Surface* src)
{
    if (src->w != src->h)
    {
        cout << "For isometric projection sprite's width and height should be equal";
        return NULL;
    }

    double root = 1 / sqrt (2);

    SDL_Surface* dst = SDL_CreateRGBSurface(    src->flags,
                                                src->w * root + 1,
                                                root * (src->w / 2 + src->h),
                                                src->format->BitsPerPixel,
                                                src->format->Rmask,
                                                src->format->Gmask,
                                                src->format->Bmask,
                                                src->format->Amask );


    for (int x = 0; x < src->w; x++)
    {
        for (int y = 0; y < src->h; y++)
        {
            setPixel(dst, Point ( root * x, root * (x / 2 + y)), getPixel(src, Point (x, y)));
        }
    }
    SDL_SaveBMP(dst, "ttt.bmp");
    return dst;
}

void IsometricField::setPixel(SDL_Surface* src, Point point, Uint32 pixel)
{
    Uint32* pixels = (Uint32*)src->pixels;
    pixels[point.y * src->w + point.x] = pixel;
}

Uint32 IsometricField::getPixel(SDL_Surface* src, Point point)
{
    Uint32* pixels = (Uint32*)src->pixels;
    return pixels[point.y * src->w + point.x];
}

IsometricField::~IsometricField() {
}

