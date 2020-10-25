#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "pixeloperations.h"
#include<err.h>
#include "BlackAndWhite.h"


SDL_Surface *image;
void GrayScale()
{

    Uint8 r, g, b, avg;
    int height = image->h;
    int width = image->w;
    Uint32 pixel;


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel = get_pixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            avg = (0.3*r + 0.59*g + 0.11*b);
            r = avg;
            g = avg;
            b = avg;
            pixel = SDL_MapRGB(image->format, r, g, b);
            put_pixel(image, j, i, pixel);
        }
        
    }
    
}




void Binarise(SDL_Surface *img)
{
    image = img;
    GrayScale();
    Uint8 r, g, b, avg;
    Uint32 pixel;
    int height = image->h;
    int width = image->w;
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel = get_pixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r<126)
            {
                r = 0;
                g = 0;
                b = 0;
            }
            else
            {
                r = 255;
                g = 255;
                b = 255;
            }
            pixel = SDL_MapRGB(image->format, r, g, b);
            put_pixel(image, j, i, pixel);
        }
            
    }

    
}