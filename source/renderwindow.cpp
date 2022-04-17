#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderwindow.h"

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h)
    : window(NULL), renderer(NULL), windowWidth(p_w), windowHeight(p_h)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        std::cout << "failed to init window haha: " << SDL_GetError() << "\n";
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_GetCurrentDisplayMode(0, &displayMode);
}

RenderWindow::~RenderWindow()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

SDL_Texture *RenderWindow::loadTexture(const char *filePath)
{
    SDL_Texture * texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);
    if(texture == NULL)
    {
        std::cout << "failed to load texture at " << filePath << SDL_GetError() << "\n";
    }
    return texture;
}
SDL_Surface* RenderWindow::loadSurface(const char *filePath)
{
    SDL_Surface* surface = NULL;
    surface = IMG_Load(filePath);
    if(surface == NULL)
    {
        std::cout << "failed to load surface at " << filePath << SDL_GetError() << "\n";
    }
    return surface;
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(SDL_Texture *texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void RenderWindow::render(SDL_Texture *texture, SDL_Rect textureRect)
{
    SDL_RenderCopy(renderer, texture, NULL, &textureRect);
}

void RenderWindow::render(SDL_Texture *texture, float p_x, float p_y)
{
    SDL_Rect textureRect;
    SDL_QueryTexture(texture, NULL, NULL, &textureRect.w,&textureRect.y);
    textureRect.x = p_x;
    textureRect.y = p_y;
    SDL_RenderCopy(renderer, texture, NULL, &textureRect);
}

void RenderWindow::render(Entity &entity)
{
    SDL_Rect srcRect;
    srcRect.x = entity.getRect().x;
    srcRect.y = entity.getRect().y;
    srcRect.w = entity.getRect().w;
    srcRect.h = entity.getRect().h;
    SDL_Rect dstRect;
    dstRect.x = entity.getX();
    dstRect.y = entity.getY();
    dstRect.w = entity.getRect().w * entity.getScaleX();
    dstRect.h = entity.getRect().h * entity.getScaleY();

    SDL_RenderCopy(renderer, entity.getTex(), &srcRect, &dstRect);
}

void RenderWindow::renderCentered(TTF_Font *font, const char *text, SDL_Colour colour, float p_x, float p_y)
{
    SDL_Surface* message = TTF_RenderText_Blended(font, text, colour);
    SDL_Texture* messageTex = SDL_CreateTextureFromSurface(renderer, message);
    SDL_Rect messageRect;
    SDL_QueryTexture(messageTex, NULL, NULL, &messageRect.w, &messageRect.h);
    messageRect.x = p_x - messageRect.w/2;
    messageRect.y = p_y - messageRect.h / 2;

    SDL_RenderCopy(renderer, messageTex, NULL, &messageRect);

    SDL_DestroyTexture(messageTex);
    SDL_FreeSurface(message);
}

void RenderWindow::setDrawColour(SDL_Color backgroundColour)
{
    SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
}

void RenderWindow::drawLine(int x1, int y1, int x2, int y2, SDL_Colour lineColour)
{
    SDL_SetRenderDrawColor(renderer, lineColour.r, lineColour.g, lineColour.b, lineColour.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void RenderWindow::drawRect(SDL_Rect rect, SDL_Colour rectColour)
{
    SDL_SetRenderDrawColor(renderer, rectColour.r, rectColour.g, rectColour.b, rectColour.a);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::drawRect(int x, int y, int w, int h, SDL_Colour rectColour)
{
    SDL_Rect rect = {(x),(y),(w),(h)};
    SDL_SetRenderDrawColor(renderer, rectColour.r, rectColour.g, rectColour.b, rectColour.a);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::setTitle(const char *title)
{
    SDL_SetWindowTitle(window, title);
}
void RenderWindow::setIcon(SDL_Surface *icon)
{
    SDL_SetWindowIcon(window, icon);
}

int RenderWindow::getWindowWidth()
{
    return windowWidth;
}

int RenderWindow::getWindowHeight()
{
    return windowHeight;
}