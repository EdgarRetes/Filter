#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            //get the value of each color of rgb from each pixel and get the average
            int bnw = round((image[row][column].rgbtRed + image[row][column].rgbtBlue + image[row][column].rgbtGreen) / (float)3);
            image[row][column].rgbtRed = bnw;
            image[row][column].rgbtBlue = bnw;
            image[row][column].rgbtGreen = bnw;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width / 2; column++)
        {
            //take the value of the pixel in a temporary variable
            RGBTRIPLE temp = image[row][column];
            //replace the value in of the pixel in [row][column] and replace it by the opposite value
            image[row][column] = image[row][(width - 1) - column];
            //replace the opposite value for the value stored in the temporary variable
            image[row][(width - 1) - column] = temp;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy_img [height][width];

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {

            float get_blue = 0;
            float get_green = 0;
            float get_red = 0;
            int pixels = 0;

            //take the surrounding pixels with this loops
            for (int i = 1; i > -2; i--)
            {
                for (int j = 1; j > -2; j--)
                {
                    //check if the surrounding pixels are in the 2d array of pixels
                    if (row - i >= 0 && row - i <= height - 1 && column - j >= 0 && column - j <= width - 1)
                    {
                        get_blue += image[row - i][column - j].rgbtBlue;
                        get_green += image[row - i][column - j].rgbtGreen;
                        get_red += image[row - i][column - j].rgbtRed;
                        pixels++;
                    }
                }
            }

            //store the values in a copy to keep the original values for the calculations
            copy_img[row][column].rgbtBlue = round(get_blue / pixels);
            copy_img[row][column].rgbtGreen = round(get_green / pixels);
            copy_img[row][column].rgbtRed = round(get_red / pixels);

        }
    }

    //replace the values from the original to the copy
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image[row][column] = copy_img[row][column];
        }
    }


    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy_img [height][width];

    //declare the 2d arrays of the g value in x and y

    int gxtable [3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gytable [3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {

            int gx_valueB = 0;
            int gy_valueB = 0;
            int gx_valueG = 0;
            int gy_valueG = 0;
            int gx_valueR = 0;
            int gy_valueR = 0;

            int y = 0;

            //same proccess of getting the surrounding values

            for (int i = 1; i > -2; i--)
            {
                int x = 0;
                for (int j = 1; j > -2; j--)
                {
                    //check if the values exist
                    if (row - i >= 0 && row - i <= height - 1 && column - j >= 0 && column - j <= width - 1)
                    {
                        //multiply the pixel for the value in gx and gy, using x and y as reference for the tables that work with the loop

                        gx_valueB += image[row - i][column - j].rgbtBlue * gxtable[y][x];

                        gy_valueB += image[row - i][column - j].rgbtBlue * gytable[y][x];

                        gx_valueG += image[row - i][column - j].rgbtGreen * gxtable[y][x];

                        gy_valueG += image[row - i][column - j].rgbtGreen * gytable[y][x];

                        gx_valueR += image[row - i][column - j].rgbtRed * gxtable[y][x];

                        gy_valueR += image[row - i][column - j].rgbtRed * gytable[y][x];
                    }
                    else
                    {
                        gx_valueB += 0;

                        gy_valueB += 0;

                        gx_valueG += 0;

                        gy_valueG += 0;

                        gx_valueR += 0;

                        gy_valueR += 0;
                    }
                    x++;
                }
                y++;
            }

            //calculations for the final value

            int final_blue = round(sqrt(pow(gx_valueB, (long) 2) + pow(gy_valueB, (long) 2)));
            int final_green = round(sqrt(pow(gx_valueG, (long) 2) + pow(gy_valueG, (long) 2)));
            int final_red = round(sqrt(pow(gx_valueR, (long) 2) + pow(gy_valueR, (long) 2)));

            //if they exceed the cap set the value to 255

            if (final_blue > 255)
            {
                final_blue = 255;
            }

            if (final_green > 255)
            {
                final_green = 255;
            }

            if (final_red > 255)
            {
                final_red = 255;
            }

            copy_img[row][column].rgbtBlue = final_blue;
            copy_img[row][column].rgbtGreen = final_green;
            copy_img[row][column].rgbtRed = final_red;
        }

    }

    //get the values from the copy to the original image

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image[row][column] = copy_img[row][column];
        }
    }
    return;
}
