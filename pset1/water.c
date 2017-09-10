#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int minutes;

    do
    {
        printf("Minutes: ");
        minutes = get_int();
    } while (minutes <= 0);

    printf("Bottles: %i\n", minutes * 12);
}
