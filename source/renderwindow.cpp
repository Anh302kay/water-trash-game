#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "renderwindow.h"

RenderWindow::RenderWindow(Uint32 flags, int p_w, int p_h)
    : screen(NULL), windowWidth(p_w), windowHeight(p_h), backgroundColour(NULL)
{
    screen = SDL_SetVideoMode(p_w, p_h, 32, flags | SDL_HWSURFACE);

    if (screen == NULL)
    {
        std::cout << "failed to init window haha: " << SDL_GetError() << "\n";
    }

    backgroundColour = SDL_MapRGB(screen->format, 0, 0, 0);

}

RenderWindow::~RenderWindow()
{
    //cleanUp();
}

void RenderWindow::cleanUp()
{
    if(screen != NULL)
    {
        SDL_FreeSurface(screen);
        screen = NULL;
    }
}

SDL_Surface* RenderWindow::loadSurface(const char *filePath)
{
    SDL_Surface* surface = NULL;
    surface = IMG_Load(filePath);
    if(surface == NULL)
    {
        std::cout << "failed to load surface at " << filePath << SDL_GetError() << "\n";
    }
    SDL_Surface* new_surface = SDL_DisplayFormatAlpha(surface);
    SDL_FreeSurface(surface);
    return new_surface;
}

void RenderWindow::clear()
{
    SDL_FillRect(screen, NULL, backgroundColour);
}

void RenderWindow::render(SDL_Surface *texture)
{
    SDL_BlitSurface(texture, NULL, screen, NULL);
}

void RenderWindow::render(SDL_Surface *texture, SDL_Rect textureRect)
{
    SDL_BlitSurface(texture, NULL, screen, &textureRect);
}

void RenderWindow::render(SDL_Surface *texture, float p_x, float p_y)
{
    SDL_Rect textureRect;
    textureRect.w = texture->w;
    textureRect.h = texture->h;
    textureRect.x = p_x;
    textureRect.y = p_y;
    SDL_BlitSurface(texture, NULL, screen, &textureRect);
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

    SDL_BlitSurface(entity.getTex(), &srcRect, screen, &dstRect);
}

void RenderWindow::renderCentered(TTF_Font *font, const char *text, SDL_Colour colour, float p_x, float p_y)
{
    SDL_Surface *message = TTF_RenderText_Blended(font, text, colour);
    SDL_Rect messageRect;
    messageRect.w = message->w;
    messageRect.h = message->h;
    messageRect.x = p_x - messageRect.w/2;
    messageRect.y = p_y - messageRect.h / 2;

    SDL_BlitSurface(message, NULL, screen, &messageRect);

    SDL_FreeSurface(message);
}

void RenderWindow::setDrawColour(SDL_Color colour)
{
    backgroundColour = SDL_MapRGB(screen->format, colour.r, colour.g, colour.b);
}

void RenderWindow::drawLine(int x1, int y1, int x2, int y2, SDL_Colour lineColour)
{
    backgroundColour = SDL_MapRGB(screen->format, lineColour.r, lineColour.g, lineColour.b);
    lineColor(screen, x1, y1, x2, y2, backgroundColour);
}

void RenderWindow::drawRect(SDL_Rect rect, SDL_Colour rectColour)
{
    backgroundColour = SDL_MapRGB(screen->format, rectColour.r, rectColour.g, rectColour.b);
    SDL_FillRect(screen, &rect, backgroundColour);
}

void RenderWindow::drawRect(int x, int y, int w, int h, SDL_Colour rectColour)
{
    SDL_Rect rect = {(x),(y),(w),(h)};
    backgroundColour = SDL_MapRGB(screen->format, rectColour.r, rectColour.g, rectColour.b);
    SDL_FillRect(screen, &rect, backgroundColour);
}

void RenderWindow::display()
{
    SDL_Flip(screen);
}

int RenderWindow::getWindowWidth()
{
    return windowWidth;
}

int RenderWindow::getWindowHeight()
{
    return windowHeight;
}