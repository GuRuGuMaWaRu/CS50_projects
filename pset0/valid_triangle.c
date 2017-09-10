// includes
#include <stdio.h>
#include <cs50.h>

// declarations
bool valid_triangle(float x, float y, float z);

// main program
int main(void) {
    printf("Let's check if the three sides you have form a triangle.\n");
    printf("Input an integer for the first side:");
    int x = get_int();
    printf("Input an integer for the second side:");
    int y = get_int();
    printf("Input an integer for the third side:");
    int z = get_int();

    bool result = valid_triangle(x, y, z);

    if (result)
    {
        printf("The provided sides can be used to form a triangle.\n");
    }
    else
    {
        printf("The provided sides cannot be used to form a triangle!\n");
    }
}

// valid_triangle function
bool valid_triangle(float x, float y, float z)
{
    // check for all positive sides
    if (x >= 0 || y >= 0 || z >= 0)
    {
        return false;
    }
    return x + y > z && x + z > y && y + z > x;
}
