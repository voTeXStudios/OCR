#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "pixeloperations.h"
#include<err.h>

void letter_extrapolation (SDL_Surface* image)
{
    Uint8 r, g, b;
    int check = 1;
    int iswhite;
    int min = 0, max = 0;
    int width = image->w;
    int height = image->h;
    for (int i = 0; i < width; i++)
    {
        iswhite = 0;
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (check == 1 && r != 255 && g != 255 && b != 255)
            {
                min = i - 1;
                check == 0;
                iswhite = 1;
            }
            else if (check == 0 && r != 255 && g != 255 && b != 255)
            {
                j = height;
                iswhite = 1;
            }
        }

        if (iswhite == 0)
        {
            max = i;
            Image_Formation(image, min, max);
            check = 1;
        }
        
    }
    
}

void Image_Formation(SDL_Surface *img, int min, int max)
{   
    Uint8 r, g, b;
    SDL_Surface *Newimage;
    int height = img->h;
    int width = max - min;
    Newimage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_FillRect(Newimage, NULL, SDL_MapRGB(Newimage->format, 255, 255, 255));
    for (int i = min; i < width ; i++)
    {
        for (int j = 0; j < height; j++)
        {
            
            Uint32 pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            pixel = SDL_MapRGB(Newimage->format, r, g, b);
            put_pixel(Newimage, i, j, pixel);
            
        }
               
    }
    SDL_SaveBMP(Newimage, "res.bmp");
}
