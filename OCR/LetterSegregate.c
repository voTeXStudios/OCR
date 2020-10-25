#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "pixeloperations.h"
#include "BlackAndWhite.h"
#include<err.h>
#include"LetterSegregate.h"

//void Image_Formation(SDL_Surface *img, int min, int max);

//void Image_Formation_line(SDL_Surface *img, int min, int max);

//BEGIN : Vertical Segmentation
bool check (SDL_Surface *image)
{
     Uint8 r, g, b;
     int height = image->h;
     bool is_letter = false;
      for (int j = 0; j < height; j ++)
    {
        Uint32 pixel = get_pixel(image, 0, j);
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
        if (r == 0 && g == 0 && b == 0)
        {
            is_letter = true;
            break;
        }

    }

    return is_letter;
}


void Vertical_segmentation (SDL_Surface *image)
{
    Uint8 r, g, b;
    int width = image->w;
    int height = image->h;
    int min = 0 , max = 0;
    bool is_letter = check(image);
    bool is_all_white;
    for (int i = 0; i < width; i++)
    {
        is_all_white = true;
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r == 0 && g == 0 && b == 0)
            {
                if (is_letter)
                {
                    j = height;
                    is_all_white = false;
                }
                else if (is_letter == false)
                {
                    min = i - 1;
                    j = height;
                    is_letter = true;
                    is_all_white = false;
                }
            }
        }

        if (is_all_white && is_letter)
        {
            max = i + 1;
            Image_Formation(image, min, max);
            is_letter = false;
        
            
        }

        if (i + 1 == width)
        {
            max = i;
            Image_Formation(image, min, max);
            
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
    for (int i = 0; i < width ; i++)
    {
        for (int j = 0; j < height; j++)
        {
            
            Uint32 pixel = get_pixel(img, i + min, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            pixel = SDL_MapRGB(Newimage->format, r, g, b);
            put_pixel(Newimage, i, j, pixel);
            
        }
               
    }

    SDL_SaveBMP(Newimage, "res.bmp");
}

//END : Vertical Segmentation


/* WORK IN PROGRESS .........

void Line_Segmentation(SDL_Surface *image)
{
    Uint8 r, g, b;
    int width = image->w;
    int height = image->h;
    int min = 0, max = 0;
    bool is_all_white;
    bool is_word = true;
    for (int i = 0; i < height; i++)
    {
        is_all_white = true;
        for (int j = 0; j < width; j++)
        {
            if (r == 0 && g == 0 && b == 0)
            {
                if (is_word)
                {
                    j = width;
                    is_all_white = false;
                }
                else if (is_word == false)
                {
                    min = i;
                    is_word = true;
                    is_all_white = false;
                    j = width;
                }
            }
        }
        
        if (is_all_white && is_word)
        {
            max = i;
            Image_Formation_line(image, min, max);
            is_word = false;
            break;
        }

        if (i + 1 == height)
        {
            max = i;
            Image_Formation_line(image, min, max);
        }

    }
    

}

void Image_Formation_line(SDL_Surface *img, int min, int max)
{   
    Uint8 r, g, b;
    SDL_Surface *Newimage;
    int width = img->w;
    int height = max - min;
    Newimage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_FillRect(Newimage, NULL, SDL_MapRGB(Newimage->format, 255, 255, 255));
    for (int i = 0; i < width ; i++)
    {
        for (int j = 0; j < height; j++)
        {
            
            Uint32 pixel = get_pixel(img, i, j + min);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            pixel = SDL_MapRGB(Newimage->format, r, g, b);
            put_pixel(Newimage, i, j, pixel);
            
        }
               
    }

    SDL_SaveBMP(Newimage, "res.bmp");
}
*/
