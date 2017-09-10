#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    // remember input file name
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // create buffer file
    BYTE *buffer = malloc(512);
    if (buffer == NULL)
    {
        fprintf(stderr, "Out of memory.\n");
        return 3;
    }

    // create loop condition variable
    int block_size = fread(buffer, 512, 1, inptr);
    // create file counter variable
    int file_counter = 0;
    // create file variables
    char filename[8];
    FILE *img;

    // start loop
    while(block_size == 1)
    {
        // start writing new JPEG
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // special conditions if this is the first JPEG
            if (file_counter == 0)
            {
                sprintf(filename, "%03i.jpg", file_counter);
                img = fopen(filename, "w");
            }
            // normal conditions for all JPEGs after the first
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", file_counter);
                img = fopen(filename, "w");
            }

            file_counter = file_counter + 1;
        }

        // write data if there is an open JPEG file
        if (file_counter > 0)
        {
            fwrite(buffer, 512, 1, img);
        }

        block_size = fread(buffer, 512, 1, inptr);
    }

    // close last JPEG
    fclose(img);

    // close input file
    fclose(inptr);

    // free memalloc
    free(buffer);

    return 0;

}
