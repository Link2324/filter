#include "helpers.h"
#include <math.h>

#include <stdlib.h>

typedef struct
{
    int red;
    int green;
    int blue;
}
kernel;
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int bw;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            bw = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3);
            image[i][j].rgbtBlue = bw;
            image[i][j].rgbtGreen = bw;
            image[i][j].rgbtRed = bw;
        }

    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][width - j - 1];
            image[i][width - j - 1] = image[i][j];
            image[i][j] = temp;
        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel[height][width];

    for (int p = 0; p < height; p++)
    {
        for (int q = 0; q < width; q++)
        {
            pixel[p][q] = image[p][q]; // copying rgb array
        }
    }
    int sum_red, sum_green, sum_blue, count;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            count = 0;
            sum_red = 0;
            sum_green = 0;
            sum_blue = 0;
            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    if ((i + m < 0) || (j + n < 0) || ((i + m) > height - 1) || ((j + n) > width - 1))
                    {
                        continue;
                    }
                    else
                    {
                        sum_red += pixel[i + m][j + n].rgbtRed;
                        sum_green += pixel[i + m][j + n].rgbtGreen;
                        sum_blue += pixel[i + m][j + n].rgbtBlue;
                        count++;
                    }
                }
            }
            image[i][j].rgbtRed = round((float)sum_red / count);
            image[i][j].rgbtGreen = round((float)sum_green / count);
            image[i][j].rgbtBlue = round((float)sum_blue / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel[height][width];
    int G_x[3][3] = {
        {-1, 0 , 1},
        {-2, 0 , 2},
        {-1, 0 , 1}
    };
    int G_y[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    kernel x, y, net;

    for (int p = 0; p < height; p++)
    {
        for (int q = 0; q < width; q++)
        {
            pixel[p][q] = image[p][q];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            x = (kernel){.red = 0, .blue = 0, .green = 0};
            y = (kernel){.red = 0, .blue = 0, .green = 0};
            net = (kernel){.red = 0, .blue = 0, .green = 0};
            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    if ((i + m < 0) || (j + n < 0) || ((i + m) > height - 1) || ((j + n) > width - 1))
                    {
                        continue;
                    }
                    x.red += pixel[i + m][j + n].rgbtRed * G_x[1 + m][1 + n];
                    x.blue += pixel[i + m][j + n].rgbtBlue * G_x[1 + m][1 + n];
                    x.green += pixel[i + m][j + n].rgbtGreen * G_x[1 + m][1 + n];

                    y.red += pixel[i + m][j + n].rgbtRed * G_y[1 + m][1 + n];
                    y.blue += pixel[i + m][j + n].rgbtBlue * G_y[1 + m][1 + n];
                    y.green += pixel[i + m][j + n].rgbtGreen * G_y[1 + m][1 + n];
                }
            }
            net = (kernel){
                .red = round(sqrt(abs((x.red * x.red) + (y.red * y.red)))),
                .green = round(sqrt(abs((x.green * x.green) + (y.green * y.green)))),
                .blue =round(sqrt(abs((x.blue * x.blue) + (y.blue * y.blue)))),
            };
            if (net.red > 255)
            {
                net.red = 0;
            }
            if (net.blue > 255)
            {
                net.blue = 0;
            }
            if (net.green > 255)
            {
                net.green = 0;
            }
            image[i][j].rgbtRed = 255 - net.red;
            image[i][j].rgbtGreen = 255 - net.green;
            image[i][j].rgbtBlue = 255 - net.blue;
        }
    }
    return;
}
