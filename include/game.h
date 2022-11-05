#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL2_framerate.h>
#include <SDL2_gfxPrimitives.h>


#include "grid.h"

struct Shape
{
    // Shape coordinates
    int w,h;
    int blocks[4];
    int start[4];
    SDL_Color color;

};
typedef struct Shape Shape;


void Generate_shape(int type,Shape *shape);
bool Game_start(SDL_Renderer *renderer, int w, int h);


#endif // GAME_H
