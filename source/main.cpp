#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "renderwindow.h"
#include "entity.h"

#define screenWidth 800
#define screenHeight 600

bool SDLinit()
{
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
unsigned int trashVisibility;
int timer = 0;
Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;
RenderWindow window("Garbage Gatherer", screenWidth, screenHeight);

SDL_Colour MapToColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Colour colour = {(r),(g),(b),(a)};
    return colour;
}

SDL_Rect MapToRect(int x, int y, int w, int h)
{
    SDL_Rect rect = {(x), (y), (w), (h)};
    return rect;
}

void randomizeTrash(Entity& trash)
{
    trash.setXY(rand() % (screenWidth/2 - 60 + trash.getRect().w/2)*2, rand() % (screenHeight - 230) + 120);
}

bool trashCollision(Entity& trash, Entity& hook)
{
    SDL_Rect trashRect = {((int)trash.getX()), ((int)trash.getY()), (trash.getRect().w ), (trash.getRect().h)};
    SDL_Rect hookRect = {((int)hook.getX()), ((int)hook.getY()), (hook.getRect().w ), (hook.getRect().h )};
    trashRect.h *= trash.getScaleY();
    trashRect.w *= trash.getScaleX();
    hookRect.h *= hook.getScaleY();
    hookRect.w *= hook.getScaleX();


    return (SDL_HasIntersection(&trashRect, &hookRect));

}

void reset(std::vector<Entity>& tt)
{
    for (Entity &t : tt)
    {
        switch (std::rand() % 5 + 1)
        {
        case 1:
            t.setTex(window.loadTexture("res/gfx/can2.png"));
            break;
        case 2:
            t.setTex(window.loadTexture("res/gfx/bottle.png"));
            break;
        case 3:
            t.setTex(window.loadTexture("res/gfx/phone.png"));
            break;
        case 4:
            t.setTex(window.loadTexture("res/gfx/trash.png"));
            break;
        case 5:
            t.setTex(window.loadTexture("res/gfx/bag2.png"));
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
    SDL_Surface* icon = window.loadSurface("res/gfx/hook.png");
    window.setIcon(icon);
    SDL_FreeSurface(icon);

    TTF_Font* font30 = TTF_OpenFont("res/font/font.ttf", 30);
    TTF_Font* font50 = TTF_OpenFont("res/font/font.ttf", 50);
    TTF_Font* font70 = TTF_OpenFont("res/font/font.ttf", 70);

    Mix_Chunk* trashPickUp = Mix_LoadWAV("res/sound/switch26.ogg");
    Mix_Chunk* trashRelease = Mix_LoadWAV("res/sound/switch14.ogg");
    Mix_Chunk* click = Mix_LoadWAV("res/sound/click4.ogg");
    Mix_Chunk *jingle = Mix_LoadWAV("res/sound/jingles_STEEL15.ogg");

    Entity hook(window.loadTexture("res/gfx/hook.png"), 100 , 100);
    hook.setScale(0.5, 0.5);

    Entity retry(window.loadTexture("res/gfx/retry.png"), screenWidth/2, 500);
    retry.setXY(screenWidth/2 - retry.getRect().w/2, 350);
    Entity ocean(window.loadTexture("res/gfx/ocean.png"), 0,0);
    std::vector<Entity> trash(20);
    
    for (Entity &t : trash)
    {
        switch (std::rand() % 5 + 1)
        {
            case 1:
                t.setTex(window.loadTexture("res/gfx/can2.png"));
                break;
            case 2:
                t.setTex(window.loadTexture("res/gfx/bottle.png"));
                break;
            case 3:
                t.setTex(window.loadTexture("res/gfx/phone.png"));
                break;
            case 4:
                t.setTex(window.loadTexture("res/gfx/trash.png"));
                break;
            case 5:
                t.setTex(window.loadTexture("res/gfx/bag2.png"));
                break;
        }
        randomizeTrash(t);
    }

    SDL_Event e;
    bool gameRunning = true;
    bool isMenu = true;
    while(gameRunning)
    {
        if(isMenu)
        {
            //menu screen
            SDL_PumpEvents();
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                    gameRunning = false;
                if (e.type == SDL_MOUSEBUTTONDOWN)
                    isMenu = false;
            }
            ocean.setXY(ocean.getX(), sin(SDL_GetTicks() / 100) * 5 -70);

            //draws everything
            window.setDrawColour(MapToColour(99, 155, 255, 0));
            window.clear();

            window.render(ocean);
            window.renderCentered(font70, "Garbage Gatherer", MapToColour(0,0,0,0), screenWidth/2, 120);
            window.renderCentered(font30, "Click to play", MapToColour(0,0,0,0), screenWidth/2, 420);
            
            window.display();
        }
        else
        {
            const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
            //calculates deltatime
            lastTick = currentTick;
            currentTick = SDL_GetPerformanceCounter();
            deltaTime = (double)((currentTick - lastTick) * 1000 / (double)SDL_GetPerformanceFrequency());
            //increases timer if trash is not invisible
            if(trashVisibility != trash.size())
            timer += 1;

            //gets inputs
            SDL_PumpEvents();
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT)
                gameRunning = false;
                if(e.type == SDL_MOUSEBUTTONDOWN)
                {
                    for (Entity &t : trash)
                    {
                        if(!t.isGrabbed() && !hook.isGrabbed())
                        {
                            
                            if (t.getY() > 100)
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
                            if(t.getY() < 100)
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
                        SDL_Rect retryRect = {(int)retry.getX(), (int)retry.getY(), retry.getRect().w, retry.getRect().h};
                        SDL_Rect mosueRect = MapToRect(mouseX, mouseY, 2, 2);
                        if (SDL_HasIntersection(&retryRect, &mosueRect))
                        {
                            Mix_PlayChannel(-1, click, 0);
                            increaseTrash(trash, 10);
                            reset(trash);
                        }
                    }
                }
                if(e.type == SDL_KEYDOWN)
                {
                    if(keyboard_state_array[SDL_SCANCODE_ESCAPE])
                    gameRunning = false;
                }
            }
      
            hook.setXY(mouseX - (hook.getRect().w * hook.getScaleX()) / 2, mouseY - (hook.getRect().h * hook.getScaleY())/2 + (40 * hook.getScaleY()));
            ocean.setXY(ocean.getX(), sin(SDL_GetTicks()/100)*5 + 10);
            //clears screen and draws everything
            window.setDrawColour(MapToColour(99, 155, 255, 0));
            window.clear();



            for (Entity &t : trash)
            {   
                if(!t.isGrabbed())
                {
                    window.render(t);
                }
            }

            window.render(hook);
            for (Entity &t : trash)
            {
                if (t.isGrabbed())
                {
                    t.setXY(hook.getX(), hook.getY() + 20);
                    window.render(t);
                }
            }
            
            window.drawRect(mouseX-2, mouseY-4,5, -600, MapToColour(0, 0, 0, 0));
            window.renderCentered(font50, std::to_string(timer /window.getDisplayMode().refresh_rate).c_str(), MapToColour(0, 0, 0, 0), screenWidth/2, 50);

            window.render(ocean);
            if (trashVisibility == trash.size())
            {
                SDL_Rect retryRect = {(int)retry.getX(), (int)retry.getY(), retry.getRect().w, retry.getRect().h};
                SDL_Rect mosueRect = MapToRect(mouseX, mouseY, 2, 2);
                if (SDL_HasIntersection(&retryRect, &mosueRect))
                {
                    retry.setColourMod(127,127,127);
                }
                else
                {
                    retry.setColourMod(255, 255, 255);
                }
                window.render(retry);
            }
            window.display();
        }
        
       

    }
    Mix_FreeChunk(jingle);
    Mix_FreeChunk(click);
    Mix_FreeChunk(trashRelease);
    Mix_FreeChunk(trashPickUp);
    TTF_CloseFont(font30);
    TTF_CloseFont(font50);
    TTF_CloseFont(font70);
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}