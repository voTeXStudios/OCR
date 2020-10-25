#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<err.h>




#include "pixeloperations.h"
#include "CropImage.h"
#include "CharDetection.h"


SDL_Surface *image;
int minLeft;
int minRight;
int minTop;
int minBot;


//Drawing a horizontal Line to seperate lines of the texts
void DrawHorLine(int i)
{
    Uint32 pixel;
    int width = image->w;
    for (int j = 0; j < width; j++)
    {
        pixel = SDL_MapRGB(image->format, 255, 0, 0);
        put_pixel(image, j, i, pixel);
    }
    
}


//Drawing vertical lines to seperate characters of the text
void DrawVerLine(int x, int start, int end)
{
    Uint32 pixel;

    for (int i = start; i <= end; i++)
    {
        pixel = SDL_MapRGB(image->format, 255, 0, 0);
        put_pixel(image, x, i, pixel);
    }
    

}



void CharacterLineFormation(int start, int end)
{
    Uint8 r, g, b;
    int width = image->w;
    int height = end;
    Uint32 pixel;
    bool hasBlack;
    bool check = false;
    int y = 0;

    for (int i = 0; i < width; i++)
    {
        hasBlack = false;
        for (int j = start; j <= height; j++)
        {
            pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r != 255 && g != 255 && b  != 255)
            {
                hasBlack = true;
                check = true;
                y = i;
            }   
        }
        if (!hasBlack && check)
        {
            check = false;
            DrawVerLine(y, start, end);
        }
        
    }
    
}


/*void LineSegregation( int start, int end, int nb)
{
    Uint8 r, g, b;
    int width = image->w;
    int height = end - start;
    SDL_Surface* LineImage;
    LineImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_FillRect(LineImage, NULL, SDL_MapRGB(LineImage->format, 255, 255, 255));

    printf("ImageFormation has Started\n");
    for (int i = start; i < end ; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            Uint32 pixel = get_pixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            pixel = SDL_MapRGB(LineImage->format, r, g, b);
            put_pixel(LineImage, j, i, pixel);
        }
           
    }

    //SDL_FreeSurface(LineImage);
    printf("PictureFormed\n");
    /*char str[20];
    char src[20] = "Lines";
    sprintf(str, "%u", nb);
    strcat(src, str);
    strcat(src, ".bmp");
    SDL_SaveBMP(LineImage, src);
    
}*/



void CharacterDetection(SDL_Surface *img)
{
    image = img;
    Uint8 r, g, b;
    int width = image->w;
    int height = image->h;
    bool hasBlack;
    bool check = false;
    Uint32 pixel;

    int y = 0;
    int h = 0;
    


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
            y += 1;
            DrawHorLine(y);
            CharacterLineFormation(h, y);
            h = y;
           
        }
        
    }
    
    
}



//CharacterDetection

void DetectCharacter(SDL_Surface *img)
{
    CharacterDetection(img);
}


