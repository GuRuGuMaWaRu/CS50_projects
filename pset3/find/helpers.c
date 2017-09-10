/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <string.h>
#include "helpers.h"

int bubble_sort (int values[], int n);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // if only one value is left, check if it is the value we are looking for
    if (n == 1 && values[0] != value)
    {
        return false;
    }

    // find the middle point of a current array
    int middle = n % 2 == 0 ? n / 2 : (n - 1) / 2;

    // return TRUE if middle point value is the value we seek
    if (values[middle] == value)
    {
        return true;
    }

    // else decide which half of the sorted array to search next
    if (values[middle] > value)
    {
        // search the left half of the array
        int left_half[middle];

        memcpy(left_half, values, middle * sizeof(int));

        return search(value, left_half, middle);
    }

    {
        // search the right half of the array
        int right_half[n - middle];

        memcpy(right_half, &values[middle + 1], (n - (middle + 1)) * sizeof(int));

        return search(value, right_half, n - (middle + 1));
    }

}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // do bubble sort

    // initialize SWAPPED value to check if there was a swap during a single pass
    bool swapped;

    do {
        // set SWAPPED value to false at the start of the next pass
        swapped = false;

        for (int counter = 0; counter < n - 1; counter++)
        {
            int current = values[counter];
            int next = values[counter + 1];

            // if previous value is higher than the next one, swap them
            if (current > next)
            {
                values[counter + 1] = current;
                values[counter] = next;
                // set SWAPPED value to TRUE to indicate that there was a swap during this pass
                swapped = true;
            }
        }
    }
    // continue sorting until there is no swap during a single pass
    while (swapped);
}
