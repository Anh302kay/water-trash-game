#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "entity.h"

class RenderWindow
{
public:
    RenderWindow(const char *p_title, int p_w, int p_h);
    ~RenderWindow();
    SDL_Texture *loadTexture(const char *filePath);
    SDL_Surface* loadSurface(const char* filePath);
    void clear();
    void render(SDL_Texture *texture);
    void render(SDL_Texture *texture, SDL_Rect textureRect);
    void render(SDL_Texture *texture, float p_x, float p_y);
    void render(Entity& entity);
    void renderCentered(TTF_Font* font, const char* text, SDL_Colour colour, float p_x, float p_y);
    void setTitle(const char *title);
    void setIcon(SDL_Surface *icon);
    void drawLine(int x1, int y1, int x2, int y2, SDL_Colour lineColour);
    void drawRect(SDL_Rect rect, SDL_Colour rectColour);
    void drawRect(int x, int y, int w, int h, SDL_Colour rectColour);
    void setDrawColour(SDL_Color backgroundColour);
    void display();
    SDL_DisplayMode getDisplayMode() {return displayMode; }
    int getWindowWidth();
    int getWindowHeight();
private:
SDL_Window* window;
SDL_Renderer* renderer;
SDL_DisplayMode displayMode;
const int windowWidth = 800;
const int windowHeight = 600;
};