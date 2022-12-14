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

#include "game.h"

void Generate_shape(int type, Shape *shape) {
    SDL_Color colours[7] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_ORANGE, COLOR_YELLOW};
    SDL_Color color = colours[rand() % 5];
    switch (type) {
        case 0:
            shape->w = 4;
            shape->h = 1;

            shape->blocks[0] = 4;

            shape->start[0] = 0;

            shape->color = color;
            break;
        case 1:
            shape->w = 2;
            shape->h = 2;

            shape->blocks[0] = 2;
            shape->blocks[1] = 2;

            shape->start[0] = 0;
            shape->start[1] = 0;

            shape->color = color;
            break;
        case 2:
            shape->w = 3;
            shape->h = 2;

            shape->blocks[0] = 1;
            shape->blocks[1] = 3;

            shape->start[0] = 0;
            shape->start[1] = 0;

            shape->color = color;
            break;
        case 3:
            shape->w = 3;
            shape->h = 2;
            shape->blocks[0] = 1;
            shape->blocks[1] = 3;

            shape->start[0] = 2;
            shape->start[1] = 0;

            shape->color = color;
            break;
        case 4:
            shape->w = 3;
            shape->h = 2;
            shape->blocks[0] = 2;
            shape->blocks[1] = 2;

            shape->start[0] = 1;
            shape->start[1] = 0;

            shape->color = color;
            break;
        case 5:
            shape->w = 3;
            shape->h = 2;
            shape->blocks[0] = 1;
            shape->blocks[1] = 3;

            shape->start[0] = 1;
            shape->start[1] = 0;

            shape->color = color;
            break;
        case 6:
            shape->w = 3;
            shape->h = 2;
            shape->blocks[0] = 2;
            shape->blocks[1] = 2;

            shape->start[0] = 0;
            shape->start[1] = 1;

            shape->color = color;
            break;
    }
}

void flipShape(Shape *shape) {
    //Invert height and width
    int shapeGrid[4][4] = {0};

    for (int i = 0; i < shape->h; ++i) {
        for (int j = shape->start[i]; j < (shape->start[i] + shape->blocks[i]); ++j) {
            shapeGrid[i][j] = 1;
        }

    }
    int tempW = shape->w;
    shape->w = shape->h;
    shape->h = tempW;
    //Flip shape grid
    for (int x = 0; x < 2; x++) {
        for (int y = x; y < 3 - x; y++) {
            int temp = shapeGrid[x][y];

            shapeGrid[x][y] = shapeGrid[y][3 - x];
            shapeGrid[y][3 - x] = shapeGrid[3 - x][3 - y];
            shapeGrid[3 - x][3 - y] = shapeGrid[3 - y][x];
            shapeGrid[3 - y][x] = temp;


        }
    }

    //Print shape for debugging
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%i ", shapeGrid[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //Clear everything
    memset(shape->blocks, 0, sizeof shape->blocks);
    memset(shape->start, 0, sizeof shape->start);

    //New settings for the flipped shape
    int s = 0;
    for (int i = 0; i < 4; i++) {
        int blocks = 0, start = 0;
        for (int j = 0; j < 4; j++) {
            if (shapeGrid[i][j]) blocks++;
            if (!shapeGrid[i][j] && blocks == 0) start++;
        }
        if (blocks == 0) {
            s++;
            continue;
        } else {
            shape->blocks[i - s] = blocks;
            shape->start[i - s] = start;
        }

    }


}

//Color and uncolor functions
void colorBlocks(Shape shape, Grid *grid, int fallingBrickX, int fallingBrickY) {
    for (int i = 0; i < shape.h; ++i) {
        for (int j = 0; j < shape.blocks[i]; ++j) {
            grid->cells[fallingBrickX + j + shape.start[i]][fallingBrickY + i].rectColor = shape.color;
        }
    }
}

void uncolorBlocks(Shape shape, Grid *grid, int fallingBrickX, int fallingBrickY) {
    for (int i = 0; i < shape.h; ++i) {
        for (int j = 0; j < shape.blocks[i]; ++j) {
            grid->cells[fallingBrickX + j + shape.start[i]][fallingBrickY + i].rectColor = grid->backgroundColor;
        }
    }
}


bool Game_start(SDL_Renderer *renderer, int w, int h) {
    // Init grid
    Grid grid = {0};

    // Set number of cells
    grid.xCells = 10;
    grid.yCells = 20;


    // Set grid dimensions
    int margin = 50;
    int cell_size = MIN((w - margin * 2) / grid.xCells,
                        (h - margin * 2) / grid.yCells);
    grid.rect.w = cell_size * grid.xCells;
    grid.rect.h = cell_size * grid.yCells;

    // Set grid backgroud
    grid.backgroundColor = COLOR_WHITE;

    // Set grid border thickness and color
    grid.border = 4;
    grid.borderColor = COLOR_GRAY;


    // Set cells border thickness and color
    grid.cellsBorder = 0.5;
    grid.cellsBorderColor = grid.borderColor;

    // Ajust size and center
    Grid_ajustSize(&grid);
    Grid_alignCenter(&grid, w, h);

    //Colors
    int coloredCells[20] = {0};

    if (!Grid_init(&grid)) {
        fprintf(stderr, "Grid fail to initialize !\n");
        return false;
    }

    // Initialize framerate manager : 30 FPS
    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 30);

    // Initialize start time (in ms)
    long long last = Utils_time();


    //Init shape
    Shape shape;

    //Score
    int score = 0;
    int level = 0;
    int lines = 0;

    // Falling brick coordinates
    int fallingBrickX = grid.xCells / 2;
    int fallingBrickY = -1;
    float fallingBrickSpeed = level + 2;
    int prevX = -1;
    int prevY = -1;

    (void) prevX;




    //Random Generator
    srand((unsigned) time(&t));

    // Event loop exit flag
    bool quit = false;

    //Event pause
    bool pause = false;

    //Generate shape
    Generate_shape(rand() % 7, &shape);

    // Event loop
    while (!quit) {
        SDL_Event e;
        //Check colored blocks
        bool colored = false;
        for (int i = 0; i < shape.h; ++i) {
            for (int j = 0; j < shape.blocks[i]; ++j) {
                if (grid.cells[fallingBrickX + j + shape.start[i]][fallingBrickY + i + 1].colored) {
                    colored = true;
                }
            }
        }
        // Get available event
        while (SDL_PollEvent(&e)) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            } else if (e.type == SDL_KEYDOWN) {
                //Check if block is colored on the left
                bool coloredLeft = false;
                for (int i = 0; i < shape.h; ++i) {
                    if (grid.cells[fallingBrickX + shape.start[i] - 1][fallingBrickY + i].colored) {
                        coloredLeft = true;
                    }
                }
                //Check if colored on the right
                bool coloredRight = false;
                for (int i = 0; i < shape.h; ++i) {
                    if (grid.cells[fallingBrickX + shape.start[i] + shape.blocks[i] + 1][fallingBrickY + i].colored) {
                        coloredRight = true;
                    }
                }
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        //Pause or unpause the game
                        pause = !pause;
                        break;

                    case SDLK_RIGHT:
                        //Move shape to the right
                        if (fallingBrickY != -1 && fallingBrickX < grid.xCells - shape.w && !coloredRight) {
                            // Un-color last position
                            uncolorBlocks(shape, &grid, fallingBrickX, fallingBrickY);

                            // Color new position
                            fallingBrickX++;
                            colorBlocks(shape, &grid, fallingBrickX, fallingBrickY);
                        }
                        break;

                    case SDLK_LEFT:
                        //Move block to the left
                        if (fallingBrickY != -1 && fallingBrickX > 0 && !coloredLeft) {
                            // Un-color last position
                            uncolorBlocks(shape, &grid, fallingBrickX, fallingBrickY);

                            // Color new position
                            fallingBrickX--;
                            colorBlocks(shape, &grid, fallingBrickX, fallingBrickY);
                        }
                        break;
                    case SDLK_DOWN:
                        //Increase falling speed
                        fallingBrickSpeed += 2;
                        break;
                    case SDLK_SPACE:
                        if (!colored) {
                            // Un-color last position
                            uncolorBlocks(shape, &grid, fallingBrickX, fallingBrickY);

                            //Display shape in grid
                            flipShape(&shape);
                        }
                        break;
                }
            }
        }

        // Move the falling brick
        if (Utils_time() - last >= 1000 / fallingBrickSpeed && !pause) {

            if (fallingBrickY >= 0 && fallingBrickY != grid.yCells - shape.h && !colored) {
                // Un-color the falling brick last position
                uncolorBlocks(shape, &grid, fallingBrickX, fallingBrickY);
            }

            if (fallingBrickY < grid.yCells - shape.h && !colored) {
                // Go to next position
                fallingBrickY++;

                // Color the falling brick new position
                colorBlocks(shape, &grid, fallingBrickX, fallingBrickY);

            } else {
                if (fallingBrickY == -1) {
                    printf("GAME OVER\n");
                    quit = true;
                } else {
                    //Cleared lines
                    int clrLines = 0;

                    //Tetris mechanics
                    for (int i = 0; i < shape.h; ++i) {
                        prevY = fallingBrickY + i;
                        for (int o = 0; o < shape.blocks[i]; ++o) {
                            grid.cells[fallingBrickX + o + shape.start[i]][fallingBrickY + i].colored = true;
                            coloredCells[prevY]++;

                            //Logs
                            printf("Prev: %i || Total: %i \n", prevY, coloredCells[prevY]);

                            //Verification
                            if (coloredCells[prevY] == grid.xCells) {
                                //Increase lines
                                clrLines++;

                                //Clear
                                for (int j = 0; j < grid.xCells; ++j) {
                                    grid.cells[j][prevY].rectColor = grid.backgroundColor;
                                    grid.cells[j][prevY].colored = false;
                                }

                                //Translate
                                for (int j = prevY; j > 0; --j) {
                                    //Reset counter
                                    coloredCells[j] = 0;

                                    for (int k = 0; k < grid.xCells; ++k) {
                                        //Swap colored state
                                        bool tempColored = grid.cells[k][j].colored;
                                        grid.cells[k][j].colored = grid.cells[k][j - 1].colored;
                                        grid.cells[k][j - 1].colored = tempColored;

                                        //Swap colors
                                        SDL_Color tempColor = grid.cells[k][j].rectColor;
                                        grid.cells[k][j].rectColor = grid.cells[k][j - 1].rectColor;
                                        assign_color(&grid.cells[k][j - 1].rectColor, tempColor);
                                        //Colorise
                                        if (grid.cells[k][j].colored) {
                                            coloredCells[j]++;
                                        }
                                    }
                                }

                            }
                        }
                    }

                    //Scoring and levels
                    if (clrLines < 4 && clrLines > 0) {
                        score += (100 + 200 * (clrLines - 1)) * (level + 1);
                    } else if (clrLines == 4) {
                        score += 800 * (level + 1);
                    }
                    lines += clrLines;
                    if (lines > 10) {
                        level++;
                    }

                    //Re-generate shape
                    Generate_shape(rand() % 7, &shape);

                    // Reset position
                    fallingBrickY = -1;
                    fallingBrickX = grid.xCells / 2;
                    fallingBrickSpeed = level + 2;

                }

            }

            last = Utils_time();
        }

        // Set background color
        Utils_setBackgroundColor(renderer, COLOR_DARK_GRAY);

        // Render grid
        Grid_render(&grid, renderer);

        //Score settings
        char scoreStr[5];
        char msg[50];
        sprintf(scoreStr, "%d", score);
        snprintf(msg, sizeof(msg), "%s%s", "TETRIS GAME || SCORE: ", scoreStr);

        // Show score message
        stringRGBA(renderer, grid.rect.x + grid.xCells, grid.rect.y - 20,
                   msg,
                   COLOR_LIGHT_GRAY.r, COLOR_LIGHT_GRAY.g, COLOR_LIGHT_GRAY.b, COLOR_LIGHT_GRAY.a);


        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }

    return true;
}
