#include <iostream>
#include <SDL/SDL.h>

#include "entity.h"

Entity::Entity()
{
    entityX = 0;
    entityY = 0;
    entityTex = NULL;
}

Entity::Entity(SDL_Surface *texture, float x, float y)
    : entityTex(texture), entityX(x), entityY(y)
{
    entityRect.w = entityTex->w;
    entityRect.h = entityTex->h;
}

Entity::~Entity()
{
    free();
}

void Entity::free()
{
    if(entityTex != NULL)
    {
        SDL_FreeSurface(entityTex);
        entityTex = NULL;
    }
}

void Entity::setTex(SDL_Surface *tex)
{
    entityTex = tex;
    entityRect.w = entityTex->w;
    entityRect.h = entityTex->h;
}

void Entity::setXY(float x, float y)
{
    entityX = x;
    entityY = y;
}

void Entity::setScale(float x, float y)
{
    scaleX = x;
    scaleY = y;
}

void Entity::setGrabbed(bool grab)
{
    grabbed = grab;
}

void Entity::setColourMod(Uint8 r, Uint8 g, Uint8 b)
{
    //SDL_SetColorKey(entityTex, SDL_TRUE, SDL_MapRGB(entityTex->format, r,g,b));
}