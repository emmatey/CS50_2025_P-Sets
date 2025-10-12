#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
// an 'image' comprised of a 2D array made of "scanlines"
// each array contains RGBTriple structs one for each pixel.
// the y axis is the index of the arrays.
// the x axis are arrays of RGBTRIPLE structs

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // for each height level
    // for each element in the row,
    // add each index
    // take their average
    // set this equal to each value in sctuct.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float color_average = 0;
            color_average += image[i][j].rgbtBlue;
            color_average += image[i][j].rgbtGreen;
            color_average += image[i][j].rgbtRed;

            color_average = round(color_average / 3.0);

            image[i][j].rgbtBlue = color_average;
            image[i][j].rgbtGreen = color_average;
            image[i][j].rgbtRed = color_average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // initial idea:
    // swap each pixel with it's "mirror image"
    // 1. decide if a pixel is on the right or left edge.
    // 2. calculate how far from the closest edge the given pixel is.
    // 3. swap with the pixel at the "opposite" index.

    // floor(n/2) - 1 = "middle/stop" point.
    // Iterate through the 'vertical' array of 'scanlines'.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // 1. instantiate vars
            int mirror_idx = width - j - 1;
            RGBTRIPLE temp;
            RGBTRIPLE current = image[i][j];
            RGBTRIPLE mirror = image[i][mirror_idx];

            // 2. current into temp
            temp = current;

            // 3. mirror into current
            image[i][j] = mirror;

            // 4. temp into mirror
            image[i][mirror_idx] = temp;
        }
    }
    return;
}

// 'blur' Helper Function
RGBTRIPLE sum_valid_neighbors(int i, int j, int height, int width, RGBTRIPLE **image)
{
    float divisor = 0;
    float blu = 0;
    float grn = 0;
    float red = 0;
    // for one row below to one row above
    for (int row = -1; row < 2; row++)
    {
        // for one horizontal left to one horizontal right
        for (int col= -1; col < 2; col++)
        {
            if ((row + i) >= 0 && (row + i) < height && (col + j) >= 0 && (col + j) < width)
            {
                blu += image[row + i][col + j].rgbtBlue;
                grn += image[row + i][col + j].rgbtGreen;
                red += image[row + i][col + j].rgbtRed;
                divisor++;
            }
        }
    }
    RGBTRIPLE out_pixel;
    out_pixel.rgbtBlue = round((blu / divisor));
    out_pixel.rgbtGreen = round((grn / divisor));
    out_pixel.rgbtRed = round((red / divisor));

    return out_pixel;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{   // alloc space for an array of pointers to the first indices of the scanlines.
    // each scanline is an array of pixel structs.
    // tmp_image is a double pointer because it points to the array of pointers.
    RGBTRIPLE **tmp_image = malloc(height * sizeof(RGBTRIPLE *));
    {
        if(tmp_image == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }
    for(int i = 0; i < height; i++)
    {
        // at each index alloc an array big enough for a scanline i.e row of pixels
        // malloc returns the pointer to this array
        // therefore tmp_image is an array of pointers to other arrays.
        tmp_image[i] = (RGBTRIPLE *)malloc(width * sizeof(RGBTRIPLE));
        if(tmp_image[i] == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp_image[i][j].rgbtBlue = image[i][j].rgbtBlue;
            tmp_image[i][j].rgbtRed = image[i][j].rgbtRed;
            tmp_image[i][j].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE new_pixel = sum_valid_neighbors(i, j, height, width, tmp_image);
            image[i][j] = new_pixel;
        }
    }

    for(int i = 0; i < height; i++)
    {
        free(tmp_image[i]);
    }
    free(tmp_image);
    return;
}

// 'edges' Helper fucntion
RGBTRIPLE do_sobel(int i, int j, int height, int width, RGBTRIPLE **image)
{
    float gxBlu = 0;
    float gxGrn = 0;
    float gxRed = 0;

    float gyBlu = 0;
    float gyGrn = 0;
    float gyRed = 0;

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int row = -1, kernel_y = 0; row < 2; row++, kernel_y++)
    {
        for (int col = -1, kernel_x = 0; col < 2; col++, kernel_x++)
        {
            if ((row + i) >= 0 && (row + i) < height && (col + j) >= 0 && (col + j) < width)
            {
                gxBlu += (image[row + i][col + j].rgbtBlue) * (Gx[kernel_y][kernel_x]);
                gxGrn += (image[row + i][col + j].rgbtGreen) * (Gx[kernel_y][kernel_x]);
                gxRed += (image[row + i][col + j].rgbtRed) * (Gx[kernel_y][kernel_x]);

                gyBlu += (image[row + i][col + j].rgbtBlue) * (Gy[kernel_y][kernel_x]);
                gyGrn += (image[row + i][col + j].rgbtGreen) * (Gy[kernel_y][kernel_x]);
                gyRed += (image[row + i][col + j].rgbtRed) * (Gy[kernel_y][kernel_x]);
            }
        }
    }
    float final_blu = sqrt((gxBlu * gxBlu) + (gyBlu * gyBlu));
    float final_grn = sqrt((gxGrn * gxGrn) + (gyGrn * gyGrn));
    float final_red = sqrt((gxRed * gxRed) + (gyRed * gyRed));

    if (final_blu > 255)
    {
        final_blu = 255;
    }
    if (final_grn > 255)
    {
        final_grn = 255;
    }
    if (final_red > 255)
    {
        final_red = 255;
    }

    RGBTRIPLE out_pixel;
    out_pixel.rgbtBlue = round(final_blu);
    out_pixel.rgbtGreen = round(final_grn);
    out_pixel.rgbtRed = round(final_red);

    return out_pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE **tmp_image = malloc(height * sizeof(RGBTRIPLE *));
    {
        if(tmp_image == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }
    for(int i = 0; i < height; i++)
    {
        // at each index alloc an array big enough for a scanline i.e row of pixels
        // malloc returns the pointer to this array
        // therefore tmp_image is an array of pointers to other arrays.
        tmp_image[i] = (RGBTRIPLE *)malloc(width * sizeof(RGBTRIPLE));
        if(tmp_image[i] == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp_image[i][j].rgbtBlue = image[i][j].rgbtBlue;
            tmp_image[i][j].rgbtRed = image[i][j].rgbtRed;
            tmp_image[i][j].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE new_pixel = do_sobel(i, j, height, width, tmp_image);
            image[i][j] = new_pixel;
        }
    }
    for(int i = 0; i < height; i++)
    {
        free(tmp_image[i]);
    }
    free(tmp_image);
    return;
}
