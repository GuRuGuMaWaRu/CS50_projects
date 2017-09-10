#include <cs50.h>

int sort (int values[], int n);
int merge (int left_half[], int right_half[], int left_len, int right_len);

int main (void)
{
    int sorted[] = sort([9,8,7,6,5,4,3,2,1], 9);

    printf("Sorted arrat %i\n", sorted);
}

int sort (int values[], int n)
{
    if (n < 2)
    {
        return values[n];
    }

    int middle = n % 2 == 0 ? n / 2 : (n - 1 / 2);
    int left_half [middle];
    int right_half [n - middle];
    int counter;

    for (counter = 0; counter < n; counter++)
    {
        if (counter <= middle)
        {
            left_half[counter] = values[counter];
        }
        else
        {
            right_half[counter] = values[counter];
        }
    }

    int left = sort(left_half, middle);
    int right = sort(right_half, middle);

    // return merge(sort left half, sort right half)

    return;
}

int merge (int left_half[], int right_half[], int left_len, int right_len)
{
    if (left_len < 2)
    {
        return left_half + right_half;
    }
}
