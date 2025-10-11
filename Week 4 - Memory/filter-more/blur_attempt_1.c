
#include "helpers.h"

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
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int color_average = 0;
            color_average += image[i][j].rgbtBlue;
            color_average += image[i][j].rgbtGreen;
            color_average += image[i][j].rgbtRed;

            color_average = color_average / 3;

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
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width / 2; j++)
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
// Helper function to reduce repition
// specify the direcitons you want to include in average
RGBTRIPLE local_avg(int i, int j, int height, int width, RGBTRIPLE image[height][width],
    int N, int NE, int E, int SE, int S, int SW, int W, int NW)
    {
        // divisior initialized to 1 because there will always be at least the reference pixel.
        int divisor = 1;
        int blue_avg = 0;
        int green_avg = 0;
        int red_avg = 0;

        // Adds values of starting reference pixel
        blue_avg += image[i][j].rgbtBlue;
        green_avg += image[i][j].rgbtGreen;
        red_avg += image[i][j].rgbtRed;

        if(N == 1)
        {
            blue_avg += image[i-1][j].rgbtBlue;
            red_avg += image[i-1][j].rgbtRed;
            green_avg += image[i-1][j].rgbtGreen;
            divisor ++;
        }
        if(NE == 1)
        {
            blue_avg += image[i-1][j+1].rgbtBlue;
            red_avg += image[i-1][j+1].rgbtRed;
            green_avg += image[i-1][j+1].rgbtGreen;
            divisor ++;
        }
        if (E == 1)
        {
            blue_avg += image[i][j+1].rgbtBlue;
            red_avg += image[i][j+1].rgbtRed;
            green_avg += image[i][j+1].rgbtGreen;
            divisor ++;
        }
        if (SE == 1)
        {
            blue_avg += image[i+1][j+1].rgbtBlue;
            red_avg += image[i+1][j+1].rgbtRed;
            green_avg += image[i+1][j+1].rgbtGreen;
            divisor ++;
        }
        if (S == 1)
        {
            blue_avg += image[i+1][j].rgbtBlue;
            red_avg += image[i+1][j].rgbtRed;
            green_avg += image[i+1][j].rgbtGreen;
            divisor ++;
        }
        if (SW == 1)
        {
            blue_avg += image[i+1][j-1].rgbtBlue;
            red_avg += image[i+1][j-1].rgbtRed;
            green_avg += image[i+1][j-1].rgbtGreen;
            divisor ++;
        }
        if (W == 1)
        {
            blue_avg += image[i][j-1].rgbtBlue;
            red_avg += image[i][j-1].rgbtRed;
            green_avg += image[i][j-1].rgbtGreen;
            divisor ++;
        }
        if (NW == 1)
        {
            blue_avg += image[i-1][j-1].rgbtBlue;
            red_avg += image[i-1][j-1].rgbtRed;
            green_avg += image[i-1][j-1].rgbtGreen;
            divisor ++;
        }
        int final_blue = blue_avg / divisor;
        int final_red = red_avg / divisor;
        int final_green = green_avg / divisor;

        RGBTRIPLE local_avg;
        local_avg.rgbtBlue = final_blue;
        local_avg.rgbtRed = final_red;
        local_avg.rgbtGreen = final_green;

        return local_avg;
    }

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Instantiate a temp copy, this prevents modifying a square and then using that modified square to calculate the subsiquent squares.
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j]; // Copy data from image to temp
        }
    }
    // Iterate through the array of scanlines
    for(int i = 0; i < height; i++)
    {
        // Iterate through each scanline
        for(int j = 0; j < width; j++)
        {
            // handle top left corner
            if(i == 0 && j == 0)
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    0, 0, 1, 1, 1, 0, 0, 0);
            }
            // handle top right corner
            else if(i == 0 && j == width - 1)
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    0, 0, 0, 0, 1, 1, 1, 0);
            }
            // handle bottom left corner
            else if(i == height - 1 && j == 0)
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    1, 1, 1, 0, 0, 0, 0, 0);
            }
            // handle bottom right corner
            else if(i == height - 1 && j == width - 1)
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    1, 0, 0, 0, 0, 0, 1, 1);
            }
            // handle left edge
            else if(j == 0)
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    1, 1, 1, 1, 1, 0, 0, 0);
            }
            // handle right edge
            else if(j == width - 1)
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    1, 0, 0, 0, 1, 1, 1, 1);
            }

            // handle top edge
            else if(i == 0)
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    0, 0, 0, 1, 1, 1, 1, 1);
            }
            // handle bottom edge
            else if(i == height - 1)
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    1, 1, 1, 0, 0, 0, 1, 1);
            }
            else
            {
                image[i][j] = local_avg(i, j, height, width, temp,\
                    1, 1, 1, 1, 1, 1, 1, 1);
            }
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
