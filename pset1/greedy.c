#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change;

    printf("O hai!");

    do
    {
        printf("How much change is owed?\n");
        change = get_float();
    } while (change < 0);

    int change_in_cents = round(change * 100);
    int change_given = 0;

    if (change_in_cents % 25 >= 0 )
    {
        change_given += floor(change_in_cents / 25);
        change_in_cents = change_in_cents % 25;
    }
    if (change_in_cents % 10 >= 0)
    {
        change_given += floor(change_in_cents / 10);
        change_in_cents = change_in_cents % 10;
    }
    if (change_in_cents % 5 >= 0)
    {
        change_given += floor(change_in_cents / 5);
        change_in_cents = change_in_cents % 5;
    }
    if (change_in_cents % 1 >= 0)
    {
        change_given += floor(change_in_cents / 1);
        change_in_cents = change_in_cents % 1;
    }

    printf("%i\n", change_given);
}
