#pragma once

#include <iostream>
#include <SDL/SDL.h>


class Entity
{
public:
    Entity();
    Entity(SDL_Surface *texture, float x, float y);
    virtual ~Entity();
    void free();
    SDL_Surface *getTex() { return entityTex; };
    void setTex(SDL_Surface *tex);
    SDL_Rect getRect() { return entityRect; }
    void setXY(float x, float y);
    float getX() { return entityX;}
    float getY() {return entityY;}
    void setScale(float x, float y);
    float getScaleX() { return scaleX; }
    float getScaleY() { return scaleY; }
    bool isGrabbed() { return grabbed; }
    void setGrabbed(bool grab);
    void setColourMod(Uint8 r, Uint8 g, Uint8 b);
    void getColourMod(Uint8 *r, Uint8 *g, Uint8 *b) { /*SDL_GetSurfaceColorMod(entityTex, r, g, b); */}

protected:
    SDL_Surface *entityTex;
    SDL_Rect entityRect = {0,0,0,0};;
    float entityX, entityY;
    float scaleX = 1.0f, scaleY = 1.0f;
    bool grabbed = false;    
};