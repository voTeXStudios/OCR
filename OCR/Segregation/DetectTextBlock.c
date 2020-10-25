#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<err.h>

#include "pixeloperations.h"
#include "CropImage.h"
#include "DetectTextBlock.h"

SDL_Surface* image;
int minLeft;
int minRight;
int minTop;
int minBot;

void Boundry(SDL_Surface *img)
{
    minLeft = CalcMinFromLeft(img);
    minRight = CalcMinFromRight(img);
    minTop = CalcMinFromTop(img);
    minBot = CalcMinFromBot(img);

    for (int i = minLeft; i <= minRight; i++)
    {
        
        Uint32 pixel = SDL_MapRGB(img->format, 255, 0, 0);
        put_pixel(img, i, minTop, pixel);
        put_pixel(img, i, minBot, pixel);
    }

    for (int j = minTop; j <= minBot; j++)
    {

        Uint32 pixel = SDL_MapRGB(img->format, 255, 0, 0);
        put_pixel(img, minLeft, j, pixel);
        put_pixel(img, minRight, j, pixel);
    }
    
    
}