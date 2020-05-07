#include "helpers.h"
#include "math.h"

int cap(double n);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculates average of RGB values and sets all to the average
            int avg = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // temporary array
            RGBTRIPLE tmp[height][width];

            // swap for blue
            tmp[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - (j + 1)].rgbtBlue;
            image[i][width - (j + 1)].rgbtBlue = tmp[i][j].rgbtBlue;

            // swap for green
            tmp[i][j].rgbtGreen = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][width - (j + 1)].rgbtGreen;
            image[i][width - (j + 1)].rgbtGreen = tmp[i][j].rgbtGreen;

            // swap for red
            tmp[i][j].rgbtRed = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][width - (j + 1)].rgbtRed;
            image[i][width - (j + 1)].rgbtRed = tmp[i][j].rgbtRed;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // make a copy of the image
    RGBTRIPLE copy[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // variables to determine size of grid around a certain pixel
            int start_row = 0;
            int start_width = 0;
            int max_row = 3;
            int max_width = 3;

            // initialize running count of each colour to 0
            int total_red = 0;
            int total_green = 0;
            int total_blue = 0;

            // keeps count of total colours summed
            int counter = 0;

            // if pixel is in first row, increment starting row position of new grid
            if (i - 1 < 0)
            {
                start_row++;
            }

            // if pixel is in last row, limits total rows of new grid to 2
            if (i + 1 == height)
            {
                max_row--;
            }

            // if pixel is in first column, increment starting width position of new grid
            if (j - 1 < 0)
            {
                start_width++;
            }

            // if pixel is in last column, limits total width of new grid to 2
            if (j + 1 == width)
            {
                max_width--;
            }

            // nested for loop to create a new grid based on variables above (i.e 2x2, 2x3, 3x2, 3x3)
            for (int k = i + start_row - 1; k < i + max_row - 1; k++)
            {
                for (int l = j + start_width - 1; l < j + max_width - 1; l++)
                {

                    // increment counter and keep running total of RGB values
                    counter++;
                    total_red += copy[k][l].rgbtRed;
                    total_green += copy[k][l].rgbtGreen;
                    total_blue += copy[k][l].rgbtBlue;
                }
            }

            // assign average surrounding values at position i,j
            image[i][j].rgbtBlue = round((float)total_blue / counter);
            image[i][j].rgbtGreen = round((float)total_green / counter);
            image[i][j].rgbtRed = round((float)total_red / counter);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // make a copy of the image
    RGBTRIPLE copy[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }

    // initialize gX and gY 3x3 array
    int arr_gx[3][3] = { { -1, 0, 1 }, {-2, 0, 2}, {-1, 0, 1} };
    int arr_gy[3][3] = { { -1, -2, -1 }, {0, 0, 0}, {1, 2, 1} };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // initialize counter for x position of arr_gx and arr_gy
            int counter1 = -1;

            // initialize running count of gX colours
            float total_red_gx = 0;
            float total_green_gx = 0;
            float total_blue_gx = 0;

            // initialize running count of gY colours
            float total_red_gy = 0;
            float total_green_gy = 0;
            float total_blue_gy = 0;

            // decrementor variable set to 0 for height and width of surrounding array
            int hdecrement = 0;
            int wdecrement = 0;

            // Increase decrementor variables if pixel is at last column or row
            if (j + 1 == width)
            {
                wdecrement++;
            }

            if (i + 1 == height)
            {
                hdecrement++;
            }

            // nested for loop to create a new grid surrounding pixel i,j
            // checks for side pixels and changes position of surround pixel and arr_gx and arr_gx counters/positions
            for (int k = i - 1; k < i + 2 - hdecrement; k++)
            {
                // initialize counter for y position of arr_gx and arr_gy
                int counter2 = 0;

                // shifts starting row of Gx and Gy arrays if pixel is in first row
                if (k < 0)
                {
                    k++;
                    counter1++;
                }

                counter1++;

                for (int l = j - 1; l < j + 2 - wdecrement; l++)
                {
                    // shifts starting column of Gx and Gy arrays if pixel is in first column
                    if (l < 0)
                    {
                        counter2++;
                    }

                    else
                    {
                        total_red_gx += (float)copy[k][l].rgbtRed * arr_gx[counter1][counter2];
                        total_green_gx += (float)copy[k][l].rgbtGreen * arr_gx[counter1][counter2];
                        total_blue_gx += (float)copy[k][l].rgbtBlue * arr_gx[counter1][counter2];

                        total_red_gy += (float)copy[k][l].rgbtRed * arr_gy[counter1][counter2];
                        total_green_gy += (float)copy[k][l].rgbtGreen * arr_gy[counter1][counter2];
                        total_blue_gy += (float)copy[k][l].rgbtBlue * arr_gy[counter1][counter2];
                        counter2++;

                    }
                }
            }

            // sobel operator equation
            image[i][j].rgbtRed = (int) cap(sqrt(pow(total_red_gx, 2) + pow(total_red_gy, 2)));
            image[i][j].rgbtGreen = (int) cap(sqrt(pow(total_green_gx, 2) + pow(total_green_gy, 2)));
            image[i][j].rgbtBlue = (int) cap(sqrt(pow(total_blue_gx, 2) + pow(total_blue_gy, 2)));

        }
    }

    return;
}

int cap(double n)
{
    int result;

    if (n >= 255)
    {
        result = 255;
    }
    else if (n < 0)
    {
        result = 0;
    }
    else
    {
        result = (int) round(n);
    }
    return (result);
}

