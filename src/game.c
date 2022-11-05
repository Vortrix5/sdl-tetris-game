#include "game.h"

void Generate_shape(int type,Shape *shape){
    switch (type) {
        case 0:
            shape->w=4;
            shape->h=1;

            shape->blocks[0]=4;

            shape->start[0]=0;

            shape->color=COLOR_GREEN;
            break;
        case 1:
            shape->w=2;
            shape->h=2;

            shape->blocks[0]=2;
            shape->blocks[1]=2;

            shape->start[0]=0;
            shape->start[1]=0;

            shape->color=COLOR_GREEN;
            break;
        case 2:
            shape->w=3;
            shape->h=2;

            shape->blocks[0]=1;
            shape->blocks[1]=3;

            shape->start[0]=0;
            shape->start[1]=0;

            shape->color=COLOR_GREEN;
            break;
        case 3:
            shape->w=3;
            shape->h=2;
            shape->blocks[0]=1;
            shape->blocks[1]=3;

            shape->start[0]=2;
            shape->start[1]=0;

            shape->color=COLOR_GREEN;
            break;
        case 4:
            shape->w=3;
            shape->h=2;
            shape->blocks[0]=2;
            shape->blocks[1]=2;

            shape->start[0]=1;
            shape->start[1]=0;

            shape->color=COLOR_GREEN;
            break;
        case 5:
            shape->w=3;
            shape->h=2;
            shape->blocks[0]=1;
            shape->blocks[1]=3;

            shape->start[0]=1;
            shape->start[1]=0;

            shape->color=COLOR_GREEN;
            break;
        case 6:
            shape->w=3;
            shape->h=2;
            shape->blocks[0]=2;
            shape->blocks[1]=2;

            shape->start[0]=0;
            shape->start[1]=1;

            shape->color=COLOR_GREEN;
            break;
    }
}


bool Game_start(SDL_Renderer *renderer, int w, int h)
{
    // Init grid
    Grid grid = {0};

    // Set number of cells
    grid.xCells = 10;
    grid.yCells = 20;


    // Set grid dimensions
    int margin = 50;
    int cell_size = MIN( (w - margin * 2) / grid.xCells,
                         (h - margin * 2) / grid.yCells );
    grid.rect.w = cell_size * grid.xCells;
    grid.rect.h = cell_size * grid.yCells;

    // Set grid backgroud
    grid.backgroundColor = COLOR_DARK_GRAY;

    // Set grid border thickness and color
    grid.border = 4;
    grid.borderColor = COLOR_GRAY;


    // Set cells border thickness and color
    grid.cellsBorder = 1;
    grid.cellsBorderColor = grid.borderColor;

    // Ajust size and center
    Grid_ajustSize(&grid);
    Grid_alignCenter(&grid, w, h);

    //Colors
    int coloredCells[20]={0};

    if(!Grid_init(&grid))
    {
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

    // Falling brick coordinates
    int fallingBrickX = grid.xCells / 2;
    int fallingBrickY = -1;
    int fallingBrickSpeed = 2;
    int prevX=-1;
    int prevY=-1;

    (void) prevX;

    //Random Generator
    srand((unsigned) time(&t));
    //int blockType=rand()%4;

    // Event loop exit flag
    bool quit = false;

    //Generate shape
    Generate_shape(rand()%7,&shape);




    // Event loop
    while(!quit)
    {
        SDL_Event e;


        // Get available event
        while(SDL_PollEvent(&e))
        {

            // User requests quit
            if(e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if(e.type == SDL_KEYDOWN)
            {
                bool coloredLeft=grid.cells[fallingBrickX-1][fallingBrickY].colored;
                bool coloredRight=grid.cells[fallingBrickX+1][fallingBrickY].colored;
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;

                case SDLK_RIGHT:
                        if(fallingBrickY != -1 && fallingBrickX < grid.xCells - shape.w && !coloredRight)
                    {
                        // Un-color last position
                        for (int i = 0; i < shape.w; ++i) {

                            for (int j = 0; j < shape.blocks[i]; ++j) {
                                grid.cells[fallingBrickX + j+ shape.start[i] ][fallingBrickY + i].rectColor = grid.backgroundColor;
                            }
                        }
                        // Color new position
                        fallingBrickX++;
                        for (int i = 0; i < shape.w; ++i) {
                            for (int j = 0; j < shape.blocks[i]; ++j) {
                                grid.cells[fallingBrickX + j + shape.start[i]][fallingBrickY + i].rectColor = shape.color;
                            }
                        }
                    }
                    break;

                case SDLK_LEFT:
                    if(fallingBrickY != -1 && fallingBrickX > 0 && !coloredLeft)
                    {
                        // Un-color last position
                        for (int i = 0; i < shape.w; ++i) {
                            for (int j = 0; j < shape.blocks[i]; ++j) {
                                grid.cells[fallingBrickX + j + shape.start[i]][fallingBrickY+ i].rectColor = grid.backgroundColor;
                            }
                        }                   // Color new position
                        fallingBrickX--;
                        for (int i = 0; i < shape.w; ++i) {
                            for (int j = 0; j < shape.blocks[i]; ++j) {
                                grid.cells[fallingBrickX + j + shape.start[i]][fallingBrickY+i].rectColor = shape.color;
                            }
                        }
                    }
                    break;
                    case SDLK_DOWN:
                        fallingBrickSpeed+=2;
                        break;
                }
            }
        }

        // Move the falling brick
        if(Utils_time() - last >= 1000 / fallingBrickSpeed)
        {
            bool colored=grid.cells[fallingBrickX][fallingBrickY+shape.h].colored;
            if(fallingBrickY >=0 && fallingBrickY!=grid.yCells - shape.h && !colored)
            {
                // Un-color the falling brick last position
                for (int i = 0; i < shape.w; ++i) {
                    for (int j = 0; j < shape.blocks[i]; ++j) {
                        grid.cells[fallingBrickX + j + shape.start[i]][fallingBrickY + i].rectColor = grid.backgroundColor;
                    }
                }
            }
            if(fallingBrickY < grid.yCells - shape.h && !colored)
            {
                // Go to next position
                fallingBrickY++;
                // Color the falling brick new position
                for (int i = 0; i < shape.w; ++i) {
                    for (int j = 0; j < shape.blocks[i]; ++j) {
                        grid.cells[fallingBrickX + j + shape.start[i]][fallingBrickY+i].rectColor = shape.color;
                    }
                }
            }
            else
            {
                if(fallingBrickY==-1){
                    printf("GAME OVER\n");
                    quit=true;
                }else {
                    //Tetris mechanics
                    for (int i = 0; i < shape.w; ++i) {
                        prevY = fallingBrickY+i;
                        for (int o = 0; o < shape.blocks[i]; ++o) {
                            grid.cells[fallingBrickX + o + shape.start[i]][fallingBrickY + i].colored = true;
                            coloredCells[prevY]++;

                            //Logs
                            printf("Prev: %i and total: %i \n",prevY,coloredCells[prevY]);

                            //Verification
                            if (coloredCells[prevY] == grid.xCells) {

                                //Clear
                                for (int j = 0; j < grid.xCells; ++j) {
                                    grid.cells[j][prevY].rectColor = grid.backgroundColor;
                                    grid.cells[j][prevY].colored = false;
                                }

                                //Translate
                                for (int j = prevY; j >= 0; --j) {
                                    //Reset counter
                                    coloredCells[j] = 0;
                                    for (int k = 0; k < grid.xCells; ++k) {
                                        bool temp = grid.cells[k][j].colored;
                                        //Swap cells
                                        grid.cells[k][j].colored = grid.cells[k][j - 1].colored;
                                        grid.cells[k][j - 1].colored = temp;
                                        //Reset Color
                                        grid.cells[k][j].rectColor = grid.backgroundColor;
                                        //Colorise
                                        if (grid.cells[k][j].colored) {
                                            grid.cells[k][j].rectColor = COLOR_GREEN;
                                            coloredCells[j]++;
                                        }
                                    }
                                }

                            }
                        }
                    }
                    // Reset position
                    fallingBrickY = -1;
                    fallingBrickX = grid.xCells / 2;
                    fallingBrickSpeed = 2;
                    //Regen Shape
                    Generate_shape(rand()%7,&shape);
                }

            }

            last = Utils_time();
        }

        // Set background color
        Utils_setBackgroundColor(renderer, COLOR_DARK_GRAY);

        // Render grid
        Grid_render(&grid, renderer);

        // Show message
        stringRGBA(renderer, grid.rect.x + grid.xCells, grid.rect.y - 20,
                   "TETRIS GAME || SCORE: 0",
                   COLOR_LIGHT_GRAY.r, COLOR_LIGHT_GRAY.g, COLOR_LIGHT_GRAY.b, COLOR_LIGHT_GRAY.a);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }

    return true;
}
