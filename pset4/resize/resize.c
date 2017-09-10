/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize resize_factor infile outfile\n");
        return 1;
    }

    // remember resize factor
    int resize_factor = atoi(argv[1]);

    // ensure proper resize factor
    if (resize_factor < 1 || resize_factor > 100)
    {
        fprintf(stderr, "Resize factor must be a positive integer less than or equal to 100\n");
        return 2;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    // calculate new padding
    int new_padding = (4 - ((bi.biWidth * resize_factor) * sizeof(RGBTRIPLE)) % 4) % 4;

    // store old image data
    int old_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int old_width = bi.biWidth;
    int old_height = bi.biHeight;


    // change outfile's BITMAPINFOHEADER
    bi.biWidth *= resize_factor;
    bi.biHeight *= resize_factor;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + new_padding) * abs(bi.biHeight);
    // change outfile's BITMAPFILEHEADER
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(old_height); i < biHeight; i++)
    {
        // repeat scanline
        for (int l = 0; l < resize_factor; l++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < old_width; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile as many times as indicated by resize_factor
                for (int m = 0; m < resize_factor; m++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // add padding to outfile new scanline
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }

            // skip back to the start of current infile scanline
            if (l < resize_factor - 1)
            {
                fseek(inptr, (old_width * 3) * -1, SEEK_CUR);
                printf("Rewinding back...\n");
            }
        }

        // skip over padding, if any
        fseek(inptr, old_padding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
