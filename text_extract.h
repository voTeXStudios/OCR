#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "pixeloperations.h"
#include "text_extract.h"
#include<err.h>






int** Matrix(SDL_Surface* image)
{
    int** array;
    int height = image->w;
    int width = image->h;
    array = malloc(sizeof(int*) * height);
    Uint8 r, g, b;
    
    for (int i = 0; i < height; i++)
    {
        array[i] = malloc(sizeof(int*) * width);
    }
    

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint32 pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            int avg = (int)(r+g+b)/3;

            array[i][j] = avg;

        }
        
    }
    return array;
    
}

int CalcMinFromLeft(SDL_Surface *image)
{
   
   Uint8 r, g, b;
   int min = INT8_MAX;
   int width = image->w;
   int height = image->h;
   for (int i = 0; i < height; i++)
   {
       for (int j = 0; j < width; j++)
       {
           Uint32 pixel = get_pixel(image, j, i);
           SDL_GetRGB(pixel, image->format, &r, &g, &b);
           //int avg = (int)(r+g+b)/3;
           if (r==0 && g==0 && b==0)
           {
               if (j < min)
                    min = j;
               
           }
           
       }
       
    }
    return min;

}

SDL_Surface* ImageFormation(SDL_Surface *img)
{   
    Uint8 r, g, b;
    int min = text_extract(img);
    SDL_Surface *Newimage;
    int width = img->w;
    int height = img->h;
    width -= min;
    Newimage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_FillRect(Newimage, NULL, SDL_MapRGB(Newimage->format, 255, 255, 255));

    

    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width; j++)
        {
            
            Uint32 pixel = get_pixel(img, j+min, i);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            pixel = SDL_MapRGB(Newimage->format, r, g, b);
            put_pixel(Newimage, j, i, pixel);
            
        }
               
    }
    SDL_SaveBMP(Newimage, "res.bmp");
}




