#include <iostream>
#include <math.h>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <3ds.h>

#include "renderwindow.h"
#include "entity.h"

#define topScreenW 400
#define topScreenH 240

#define bottomScreenW 320
#define bottomScreenH 240

bool SDLinit()
{
    romfsInit();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0)
        std::cout << "could not init sdl 2" << SDL_GetError() << "\n";
    if (!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_INT HAS FAILED SDL_ERROR: " << SDL_GetError() << "\n";
    if (TTF_Init() == -1)
        std::cout << "ttf_init HAS FAILED SDL_ERROR: " << SDL_GetError() << "\n";
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        std::cout << "sdl mixer has dfailed lol: " << Mix_GetError() << "\n";

    return true;
}

bool init = SDLinit();
unsigned int trashVisibility = 0;
int timer = 0;
RenderWindow screen(SDL_DUALSCR, 400, 480);
char timerChar[32];
char trashCollected[32];

SDL_Colour MapToColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Colour colour = {(r),(g),(b),(a)};
    return colour;
}

SDL_Rect MapToRect(Sint16 x, Sint16 y, Uint16 w, Uint16 h)
{
    SDL_Rect rect = {(x), (y), (w), (h)};
    return rect;
}

void randomizeTrash(Entity& trash)
{
    trash.setXY((rand() % 300 + 60 - trash.getRect().w), (rand() % 100) + 380 - trash.getRect().h);
}

bool trashCollision(Entity& trash, Entity& hook)
{
    SDL_Rect trashRect = {((Sint16)trash.getX()), ((Sint16)trash.getY()), (trash.getRect().w), (trash.getRect().h)};
    SDL_Rect hookRect = {((Sint16)hook.getX()), ((Sint16)hook.getY()), (hook.getRect().w), (hook.getRect().h)};
    trashRect.h *= trash.getScaleY();
    trashRect.w *= trash.getScaleX();
    hookRect.h *= hook.getScaleY();
    hookRect.w *= hook.getScaleX();

    return (trash.getX() + trashRect.w > hook.getX()) && (trash.getX() < hook.getX() + hookRect.w) && (trash.getY() + trashRect.h > hook.getY()) && (trash.getY() < hook.getY() + hookRect.h);
}

bool rectCollision(SDL_Rect rect1, SDL_Rect rect2)
{
    return (rect1.x + rect1.w > rect2.x) && (rect1.x < rect2.x + rect2.x) && (rect1.y + rect1.h > rect2.y) && (rect1.y < rect2.y + rect2.h);
}

void reset(std::vector<Entity>& tt)
{
    for (Entity &t : tt)
    {
        switch (std::rand() % 5 + 1)
        {
        case 1:
            t.setTex(screen.loadSurface("romfs:gfx/can2.png"));
            break;
        case 2:
            t.setTex(screen.loadSurface("romfs:gfx/bottle.png"));
            break;
        case 3:
            t.setTex(screen.loadSurface("romfs:gfx/phone.png"));
            break;
        case 4:
            t.setTex(screen.loadSurface("romfs:gfx/trash.png"));
            break;
        case 5:
            t.setTex(screen.loadSurface("romfs:gfx/bag2.png"));
            break;
        }
        randomizeTrash(t);
        t.setScale(1.0f,1.0f);
    }
    trashVisibility = 0;
    timer = 0;
}

void increaseTrash(std::vector<Entity>& tt, unsigned int increase)
{
    increase += tt.size();
    tt.resize(increase);
}

int main(int argc, char* argv[])
{
    srand((unsigned)time(NULL));
    SDL_ShowCursor(SDL_FALSE);

    TTF_Font* font30 = TTF_OpenFont("romfs:font/font.ttf", 15);
    TTF_Font* font50 = TTF_OpenFont("romfs:font/font.ttf", 25);
    TTF_Font* font60 = TTF_OpenFont("romfs:font/font.ttf", 30);
    TTF_Font* font70 = TTF_OpenFont("romfs:font/font.ttf", 35);

    Mix_Chunk *trashPickUp = Mix_LoadWAV("romfs:sound/switch26.ogg");
    Mix_Chunk *trashRelease = Mix_LoadWAV("romfs:sound/switch14.ogg");
    Mix_Chunk *click = Mix_LoadWAV("romfs:sound/click4.ogg");
    Mix_Chunk *jingle = Mix_LoadWAV("romfs:sound/jingles_STEEL15.ogg");

    Entity hook(screen.loadSurface("romfs:gfx/hook.png"), 100, 100);
    hook.setScale(0.5, 0.5);

    Entity retry(screen.loadSurface("romfs:gfx/retry.png"), topScreenW / 2, 500);
    retry.setXY(topScreenW / 2 - retry.getRect().w / 2, 350);
    Entity ocean(screen.loadSurface("romfs:gfx/ocean.png"), 40, 0);
    std::vector<Entity> trash(20);
    
    for (Entity &t : trash)
    {
        switch (std::rand() % 5 + 1)
        {
        case 1:
            t.setTex(screen.loadSurface("romfs:gfx/can2.png"));
            break;
        case 2:
            t.setTex(screen.loadSurface("romfs:gfx/bottle.png"));
            break;
        case 3:
            t.setTex(screen.loadSurface("romfs:gfx/phone.png"));
            break;
        case 4:
            t.setTex(screen.loadSurface("romfs:gfx/trash.png"));
            break;
        case 5:
            t.setTex(screen.loadSurface("romfs:gfx/bag2.png"));
            break;
        }
        randomizeTrash(t);
    }

    bool isMenu = true;
    while(aptMainLoop())
    {
        touchPosition touch;
        hidScanInput();
        hidTouchRead(&touch);
        u32 kDown = hidKeysDown();
        if(isMenu)
        {
            //menu screen
            if(kDown & KEY_TOUCH)
            {
                isMenu = false;
            }
            if(kDown & KEY_START)
             break;

            ocean.setXY(ocean.getX(), (sin(SDL_GetTicks() / 100) * 5 -70) + 240 + 80);
            
            //draws everything
            screen.setDrawColour(MapToColour(99, 155, 255, 0));
            screen.clear();
            screen.render(ocean);
            screen.renderCentered(font70, "Garbage Gatherer", MapToColour(0, 0, 0, 0), topScreenW / 2, 50);
            screen.renderCentered(font30, "Tap to play", MapToColour(0, 0, 0, 0), topScreenW / 2, 150);

            screen.display();
            SDL_Delay(4);
        }
        else
        {
            //increases timer if trash is not invisible
            if(trashVisibility != trash.size())
            timer += 1;

            int mouseX, mouseY;
            if(touch.px != 0)
            {
                mouseX = touch.px + 40;
                mouseY = touch.py + 240;
            }
            
            
            if(kDown & KEY_TOUCH)
            {
                for (Entity &t : trash)
                {
                    if(!t.isGrabbed() && !hook.isGrabbed())
                    {
                        if (t.getY() > 300)
                        {
                            // trash pickup
                            if (trashCollision(t, hook))
                            {
                                Mix_PlayChannel(-1, trashPickUp, 0);
                                t.setGrabbed(true);
                                hook.setGrabbed(true);
                            }
                        }
                    }
                    else if(t.isGrabbed() && hook.isGrabbed())
                    {
                        //makes trash invisibles
                        if(t.getY() < 300)
                        {
                            Mix_PlayChannel(-1, trashRelease, 0);
                            t.setGrabbed(false);
                            hook.setGrabbed(false);
                            t.setScale(0, 0);
                            trashVisibility++;
                            if(trashVisibility == trash.size())
                                Mix_PlayChannel(-1, jingle, 0);
                        }
                    }
                }
                //checks if all trash is invisible
                if (trashVisibility == trash.size())
                {
                    SDL_Rect retryRect = {(Sint16)retry.getX(), (Sint16)retry.getY(), retry.getRect().w, retry.getRect().h};
                    SDL_Rect mosueRect = MapToRect(mouseX, mouseY, 2, 2);
                    if (rectCollision(retryRect, mosueRect))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        increaseTrash(trash, 10);
                        reset(trash);
                    }
                }
            }
  
            if (kDown & KEY_START)
                break;

            hook.setXY(mouseX - hook.getRect().w / 2, mouseY - hook.getRect().h/2 + 10);
            ocean.setXY(ocean.getX(), (sin(SDL_GetTicks() / 100) * 5 - 70) + 240 + 85);

            //clears screen and draws everything
            screen.setDrawColour(MapToColour(99, 155, 255, 0));
            screen.clear();

            for (Entity &t : trash)
            {   
                if(t.getScaleX() != 0)
                if(!t.isGrabbed())
                {
                    screen.render(t);
                }
            }

            screen.render(hook);
            for (Entity &t : trash)
            {
                if (t.isGrabbed())
                {
                    t.setXY(hook.getX(), hook.getY() + 20);
                    screen.render(t);
                }
            }

            screen.drawRect(mouseX - 1, mouseY - (mouseY - 240), 3, mouseY - 240, MapToColour(0, 0, 0, 0));
            sprintf(timerChar, "%d", (timer / 60));
            screen.renderCentered(font60, "Time", MapToColour(0, 0, 0, 0), 100, 30);
            screen.renderCentered(font50, timerChar, MapToColour(0, 0, 0, 0), 100, 60);
            sprintf(trashCollected, "%d", trashVisibility);
            screen.renderCentered(font60, "Trash Collected", MapToColour(0, 0, 0, 0), 300, 30);
            screen.renderCentered(font50, trashCollected, MapToColour(0,0,0,0), 300, 60);

            screen.render(ocean);
            if (trashVisibility == trash.size())
            {
                screen.render(retry);  
            }
            screen.display();
            SDL_Delay(4);
            
        }
        
       

    }
    for (Entity &t : trash)
    {
        t.free();
    }
    hook.free();
    ocean.free();
    retry.free();

    //screen.cleanUp();

    Mix_FreeChunk(jingle);
    Mix_FreeChunk(click);
    Mix_FreeChunk(trashRelease);
    Mix_FreeChunk(trashPickUp);

    TTF_CloseFont(font30);
    TTF_CloseFont(font50);
    TTF_CloseFont(font60);
    TTF_CloseFont(font70);

    SDL_ShowCursor(SDL_ENABLE);

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    romfsExit();
    
    return 0;
}