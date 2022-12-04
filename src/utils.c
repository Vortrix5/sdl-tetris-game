#include "utils.h"

#include <time.h>


const SDL_Color COLOR_TRANSPARENT   = {0};
const SDL_Color COLOR_BLACK         = {0, 0, 0, 255};
const SDL_Color COLOR_WHITE         = {252, 252, 252, 255};
const SDL_Color COLOR_GRAY          = {100, 100, 100, 255};
const SDL_Color COLOR_DARK_GRAY     = {30, 30, 30, 255};
const SDL_Color COLOR_LIGHT_GRAY    = {200, 200, 200, 255};
const SDL_Color COLOR_RED           = {245, 42, 42, 255};
const SDL_Color COLOR_GREEN         = {102, 204, 0, 255};
const SDL_Color COLOR_BLUE          = {0, 128, 255, 255};
const SDL_Color COLOR_YELLOW        = {245, 231, 42, 255};
const SDL_Color COLOR_ORANGE        = {255, 133, 0, 255};
const SDL_Color COLOR_PINK          = {255, 0, 206, 255};
const SDL_Color COLOR_VIOLET        = {145, 0, 255, 255};

void Utils_setBackgroundColor(SDL_Renderer *renderer, SDL_Color color)
{
    // Initialize renderer color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Clear screen
    SDL_RenderClear(renderer);
}

long long Utils_time(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

void Utils_randInit(void)
{
    srand(time(NULL));
}

int Utils_rand(int min, int max)
{
    return ( rand() % (max + 1) ) + min;
}

bool Utils_equalColors(SDL_Color color1, SDL_Color color2)
{
    return *((Sint32 *) &color1) == *((Sint32 *) &color2);
}

void assign_color(SDL_Color *color1, SDL_Color color2)
{
    color1->r = color2.r;
    color1->g= color2.g;
    color1->b= color2.b;
    color1->a= color2.a;

}
