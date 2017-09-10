/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// current blank tile
int blank_y;
int blank_x;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
bool is_adjacent(int coor_y, int coor_x);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        //clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int i, j;
    int tiles_number = d * d - 1;

    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {

            if (d % 2 == 0 && tiles_number < 3)
            {
                if (tiles_number == 2) {
                    board[i][j] = 1;
                }
                else if (tiles_number == 1)
                {
                    board[i][j] = 2;
                }
            }
            else
            {
                board[i][j] = tiles_number;
            }

            if (tiles_number == 0)
            {
                blank_x = i;
                blank_y = j;
            }
            tiles_number -= 1;
        }
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    int i, j;

    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                printf(" _  ");
            }
            else
            {
                printf("%2i  ", board[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    int i, j;

    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                if (is_adjacent(i, j))
                {
                    board[i][j] = 0;
                    board[blank_y][blank_x] = tile;
                    blank_y = i;
                    blank_x = j;
                    return true;
                }
                return false;
            }
        }
        printf("\n");
    }

    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    int i, j;
    int counter = 1;

    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if (i == d - 1 && j == d - 1)
            {
                if (board[i][j] != 0)
                {
                    return false;
                }
            }
            else
            {
                if (board[i][j] != counter)
                {
                    return false;
                }
            }
            counter = counter + 1;
        }
    }
    return true;
}

bool is_adjacent(int coor_y, int coor_x)
{
    if (coor_y == blank_y)
    {
        if (coor_x == blank_x - 1 || coor_x == blank_x + 1)
        {
            return true;
        }
    }

    if (coor_y == blank_y - 1 || coor_y == blank_y + 1)
    {
        if (coor_x == blank_x)
        {
            return true;
        }
    }
    return false;
}
