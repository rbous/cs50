#include "helpers.h"
#include <math.h>
#include <stdint.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE avg;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            avg = round((image[h][w].rgbtGreen + image[h][w].rgbtRed + image[h][w].rgbtBlue) /
                        (float) 3);
            image[h][w].rgbtGreen = avg;
            image[h][w].rgbtBlue = avg;
            image[h][w].rgbtRed = avg;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            temp = image[h][w];
            image[h][w] = image[h][width - (w + 1)];
            image[h][width - (w + 1)] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy image to image2
    RGBTRIPLE image2[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image2[h][w] = image[h][w];
        }
    }

    // blur image
    int sum_r, sum_g, sum_b, count;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            sum_r = 0;
            sum_g = 0;
            sum_b = 0;
            count = 0;

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if (h + i >= 0 && h + i < height && w + j >= 0 && w + j < width)
                    {
                        sum_r += image2[h + i][w + j].rgbtRed;
                        sum_g += image2[h + i][w + j].rgbtGreen;
                        sum_b += image2[h + i][w + j].rgbtBlue;
                        count++;
                    }
                }
            }

            image[h][w].rgbtRed = round(sum_r / (float) count);
            image[h][w].rgbtGreen = round(sum_g / (float) count);
            image[h][w].rgbtBlue = round(sum_b / (float) count);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // copy image to image2
    RGBTRIPLE image2[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image2[h][w] = image[h][w];
        }
    }

    // sobel operation
    int kernel_Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernel_Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            float Gx[3] = {0, 0, 0};
            float Gy[3] = {0, 0, 0};

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {

                    if (h + i >= 0 && h + i < height && w + j >= 0 && w + j < width)
                    {
                        int kx = kernel_Gx[i + 1][j + 1];
                        int ky = kernel_Gy[i + 1][j + 1];

                        Gx[0] += image2[h + i][w + j].rgbtRed * kx;
                        Gx[1] += image2[h + i][w + j].rgbtGreen * kx;
                        Gx[2] += image2[h + i][w + j].rgbtBlue * kx;

                        Gy[0] += image2[h + i][w + j].rgbtRed * ky;
                        Gy[1] += image2[h + i][w + j].rgbtGreen * ky;
                        Gy[2] += image2[h + i][w + j].rgbtBlue * ky;
                    }
                }
            }

            int r = round(sqrt(Gx[0] * Gx[0] + Gy[0] * Gy[0]));
            int g = round(sqrt(Gx[1] * Gx[1] + Gy[1] * Gy[1]));
            int b = round(sqrt(Gx[2] * Gx[2] + Gy[2] * Gy[2]));

            if (r > 255)
            {
                image[h][w].rgbtRed = 255;
            }
            else
            {
                image[h][w].rgbtRed = r;
            }

            if (g > 255)
            {
                image[h][w].rgbtGreen = 255;
            }
            else
            {
                image[h][w].rgbtGreen = g;
            }

            if (b > 255)
            {
                image[h][w].rgbtBlue = 255;
            }
            else
            {
                image[h][w].rgbtBlue = b;
            }
        }
    }

    return;
}
