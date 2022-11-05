#include "grid.h"
#define UNUSED(x) (void)(x)


int Grid_ajustSize(Grid *grid)
{
    if(!grid->rect.w || !grid->rect.h || !grid->xCells || !grid->yCells)
    {
        fprintf(stderr, "Grid dimensions or number of cells not initialised !\n");
        return false;
    }

    // Init rect
    int interspaceWidth = grid->xCells * grid->cellsBorder * 2;
    grid->rect.w -= (grid->rect.w - (grid->border * 2) - interspaceWidth) % grid->xCells;

    int interspaceHeigth = grid->yCells * grid->cellsBorder * 2;
    grid->rect.h -= (grid->rect.h - (grid->border * 2) - interspaceHeigth) % grid->yCells;

    return true;
}

void Grid_alignCenter(Grid *grid, int screenWidth, int screenHeight)
{
    grid->rect.x = (screenWidth - grid->rect.w) / 2;
    grid->rect.y = (screenHeight - grid->rect.h) / 2;
}

bool Grid_init(Grid *grid)
{
    if(!grid->rect.w || !grid->rect.h || !grid->xCells || !grid->yCells)
    {
        fprintf(stderr, "Grid dimensions or number of cells not initialised !\n");
        return false;
    }

    if(grid->xCells > GRID_MAX_X_CELLS || grid->yCells > GRID_MAX_Y_CELLS)
    {
        fprintf(stderr, "Grid number of cells (%d,%d) is greater than (%d,%d) !\n",
                grid->xCells, grid->yCells,
                GRID_MAX_X_CELLS, GRID_MAX_Y_CELLS);
        return false;
    }

    // Init all cells
    for(int i = 0; i < grid->xCells; ++i)
    {
        for(int j = 0; j < grid->yCells; ++j)
        {
            Grid_initCell(grid,
                          &(grid->cells[i][j]),
                          i, j,
                          grid->backgroundColor,
                          grid->cellsBorderColor, false);
        }
    }

    return true;
}

void Grid_initCell(Grid *grid, Cell *cell, int i, int j, SDL_Color color, SDL_Color borderColor, bool colored)
{
    UNUSED(colored);
    // Init rect
    int interspaceWidth = grid->xCells * grid->cellsBorder * 2;
    cell->rect.w = (grid->rect.w - (grid->border * 2) - interspaceWidth) / grid->xCells;

    int interspaceHeigth = grid->yCells * grid->cellsBorder * 2;
    cell->rect.h = (grid->rect.h - (grid->border * 2) - interspaceHeigth) / grid->yCells;

    cell->rect.x = grid->rect.x + grid->border + grid->cellsBorder + (grid->cellsBorder * 2 + cell->rect.w) * i;
    cell->rect.y = grid->rect.y + grid->border + grid->cellsBorder + (grid->cellsBorder * 2 + cell->rect.h) * j;

    // Init rectColor
    cell->rectColor = color;

    // Init border
    cell->border.w = cell->rect.w + grid->cellsBorder * 2;
    cell->border.h = cell->rect.h + grid->cellsBorder * 2;
    cell->border.x = cell->rect.x - grid->cellsBorder;
    cell->border.y = cell->rect.y - grid->cellsBorder;

    // Init borderColor
    cell->borderColor = borderColor;

    //Init colored
    cell->colored=false;
}

void Grid_render(Grid *grid, SDL_Renderer *renderer)
{
    if(grid->border != 0) // Grid border thickness different from 0
    {
        // Set renderer color to draw the grid border
        SDL_SetRenderDrawColor(renderer,
                               grid->borderColor.r,
                               grid->borderColor.g,
                               grid->borderColor.b,
                               grid->borderColor.a);

        // Render grid border
        SDL_RenderFillRect(renderer, &(grid->rect));
    }

    // Render all cells
    for(int i = 0; i < grid->xCells; ++i)
    {
        for(int j = 0; j < grid->yCells; ++j)
        {
            Grid_renderCell(&(grid->cells[i][j]), renderer);
        }
    }
}

void Grid_renderCell(Cell *cell, SDL_Renderer *renderer)
{
    if(cell->border.x != cell->rect.x) // Cells border thickness different from 0
    {
        // Set renderer color to cell color
        SDL_SetRenderDrawColor(renderer,
                               cell->borderColor.r,
                               cell->borderColor.g,
                               cell->borderColor.b,
                               cell->borderColor.a);

        // Render filled cell
        SDL_RenderFillRect(renderer, &(cell->border));
    }

    // Set renderer color to cell color
    SDL_SetRenderDrawColor(renderer,
                           cell->rectColor.r,
                           cell->rectColor.g,
                           cell->rectColor.b,
                           cell->rectColor.a);

    // Render filled cell
    SDL_RenderFillRect(renderer, &(cell->rect));
}