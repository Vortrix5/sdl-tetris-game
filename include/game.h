/*
 *  MIT License
 *
 *  Copyright (c) 2022 Amine Zouaoui <amine.zouaoui2011@gmail.com>
 *  All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any
 *  person obtaining a copy of this software and associated
 *  documentation files (the "Software"), to deal in the
 *  Software without restriction, including without
 *  limitation the rights to use, copy, modify, merge,
 *  publish, distribute, sublicense, and/or sell copies of
 *  the Software, and to permit persons to whom the Software
 *  is furnished to do so, subject to the following
 *  conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

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
void flipShape(Shape *shape);
void colorBlocks(Shape shape, Grid* grid, int fallingBrickX, int fallingBrickY);
void uncolorBlocks(Shape shape, Grid* grid, int fallingBrickX, int fallingBrickY);

#endif // GAME_H
