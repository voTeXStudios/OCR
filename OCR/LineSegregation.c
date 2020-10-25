#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "pixeloperations.h"
#include "CropImage.h"
#include"LineSegregation.h"




SDL_Surface *image;
int minLeft;
int minRight;
int minTop;
int minBot;

//int **arr;




void Imageformation(int length)
{
    Uint8 r, g, b;
    int arr[length];
    Uint32 pixel;
    int width = image->w;
    int height = image->h;
    int counter = 0;
    bool hasBlack;
    bool check = false;
    int y = 0;
    int h = 0;
    ///height -= start;
    SDL_Surface *LineImage;
    //LineImage = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);
    //SDL_FillRect(LineImage, NULL, SDL_MapRGB(LineImage->format, 255, 255, 255));
    printf("Length: %u\n", length);
    for (int i = 0; i < height; i++)
    {
        hasBlack = false;
        for (int j = 0; j < width; j++)
        {
            pixel = get_pixel(image, j, i);
			SDL_GetRGB(pixel, image->format, &r, &g, &b);

            if (r != 255 && g != 255 && b != 255)
            {
                hasBlack = true;
                check = true;
                y = i;
            }
        }

        if (!hasBlack && check)
        {
            check = false;
            arr[counter] = y;
            printf("{%u, %u}\n", counter, y);
            counter += 1;
            h = y;
        }
        
    }

    h = 0;
    for (size_t i = 0; i < length; i++)
    {
        height = arr[i];
        printf("iteration: %zu\n", i);

        printf("{%u, %u}\n", h, height);
        LineImage = SDL_CreateRGBSurface(0, width, height+1-h, 32, 0, 0, 0, 0);
        if (LineImage == NULL)
        {
            printf("CreateRGBSurface failed: %s\n", SDL_GetError());
            return;
        }
        printf("%u\n", height+1-h);
        SDL_FillRect(LineImage, NULL, SDL_MapRGB(LineImage->format, 255, 255, 255));
        for (int j = 0; j < width; j++)
        {
            for (int k = h; i < height; i++)
            {
                pixel = get_pixel(image, j, k);
                SDL_GetRGB(pixel, image->format, &r, &g, &b);
                pixel = SDL_MapRGB(LineImage->format, r, g, b);
                put_pixel(LineImage, j, k, pixel);
            }
            
        }
        //char src[20];
        printf("pictureFormed\n");
        //SDL_FreeSurface(LineImage);
        /*sprintf(src, "%u", i);
        strcat(src, ".bmp");
        SDL_SaveBMP(LineImage, src);*/
        h = height;
    }
    
}



void LineSegregation(SDL_Surface *img)
{
    
    Uint8 r, g, b;
    Uint32 pixel;
    image = img;

    int width = image->w;
    int height = image->h;
    bool hasBlack;
    bool check = false;
    int y = 0;
    int h = 0;

    int nb = 0;

    for (int i = 0; i < height; i++)
    {
        hasBlack = false;
        for (int j = 0; j < width; j++)
        {
            pixel = get_pixel(image, j, i);
			SDL_GetRGB(pixel, image->format, &r, &g, &b);

            if (r != 255 && g != 255 && b != 255)
            {
                hasBlack = true;
                check = true;
                y = i;
            }
        }

        if (!hasBlack && check)
        {
            check = false;
            nb += 1;
            //printf("Hello\n");
            
            h = y;
        }
        
    }
    
    Imageformation(nb);
}


void Segregate(SDL_Surface *img)
{
    LineSegregation(img);
}