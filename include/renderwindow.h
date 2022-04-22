#pragma once

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "entity.h"

class RenderWindow
{
public:
    RenderWindow(Uint32 flags, int p_w, int p_h);
    ~RenderWindow();
    void cleanUp();
    SDL_Surface* loadSurface(const char* filePath);
    void clear();
    void render(SDL_Surface *texture);
    void render(SDL_Surface *texture, SDL_Rect textureRect);
    void render(SDL_Surface *texture, float p_x, float p_y);
    void render(Entity& entity);
    void renderCentered(TTF_Font* font, const char* text, SDL_Colour colour, float p_x, float p_y);
    void setTitle(const char *title);
    void setIcon(SDL_Surface *icon);
    void drawLine(int x1, int y1, int x2, int y2, SDL_Colour lineColour);
    void drawRect(SDL_Rect rect, SDL_Colour rectColour);
    void drawRect(int x, int y, int w, int h, SDL_Colour rectColour);
    void setDrawColour(SDL_Color colour);
    void display();
    int getWindowWidth();
    int getWindowHeight();
private:
SDL_Surface* screen;
const int windowWidth = 800;
const int windowHeight = 600;
Uint32 backgroundColour;
};