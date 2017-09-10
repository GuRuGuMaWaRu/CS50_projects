#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;

    do
    {
        printf("Height: ");
        height = get_int();
    } while (height < 0 || height > 23);

    for (int i = 0; i < height; i++)
    {
        int empty_space = height - i;
        for (int j = 0; j < empty_space; j++)
        {
            printf(" ");
        }

        printf("#");

        for (int z = 0; z < i; z++)
        {
            printf("#");
        }

        printf("\n");
    }


}
