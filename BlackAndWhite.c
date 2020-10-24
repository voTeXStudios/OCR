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

    Uint8 r, g, b;
    int height = image->h;
    int width = image->w;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint32 pixel = get_pixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            Uint8 avg = (Uint8)(0.3*(double)r + 0.59*(double)g + 0.11*(double)b);
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
    Uint8 r, g, b;
    int height = image->h;
    int width = image->w;
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint32 pixel = get_pixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            Uint8 avg = (r+g+b)/3;
            if (avg<126)
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