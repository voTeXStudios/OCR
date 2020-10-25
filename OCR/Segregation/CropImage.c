#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include "pixeloperations.h"
#include "CropImage.h"
#include<err.h>


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
           if (r != 255 && g != 255 && b != 255)
           {
               if (j < min)
                    min = j-3;
               
           }
           
       }
       
    }
    return min;

}

int CalcMinFromRight(SDL_Surface *image)
{
    Uint8 r, g, b;
    int min = INT8_MIN;
    int width = image->w;
    int height = image->h;
    for (int i = 0; i < height; i++)
    {   
        for (int j = width - 1; j >= 0; j--)
        {
            Uint32 pixel = get_pixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r != 255 && g != 255 && b  != 255)
            {
                if (j>min)
                    min = j+2;
            }
        }
        
    }
    return min;
}

int CalcMinFromTop(SDL_Surface *image)
{
	Uint8 r, g, b;
	int min = INT8_MAX;
	int width = image->w;
	int height = image->h;
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j <  height; j++)
		{
			Uint8 pixel = get_pixel(image, i, j);
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			if (r != 255 && g != 255 && b != 255)
			{
				if (j < min)
				{
					min = j-3;
				}
			}
		}
	}
  return min;
}

int CalcMinFromBot(SDL_Surface *image)
{
	Uint8 r, g, b;
	int min = INT8_MIN;
	int width = image->w;
	int height = image->h;
	for(int i = 0; i < width; i++)
	{
		for(int j = height-1; j >= 0; j--)
		{
			Uint8 pixel = get_pixel(image, i, j);
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			if (r != 255 && g != 255 && b != 255)
			{
				if (j>min)
				{
					min = j+3;
				}
			}
		}
	}
    return min;
}

SDL_Surface* CropLeft(SDL_Surface *img)
{   
    Uint8 r, g, b;
    SDL_Surface *Newimage;
    int min = CalcMinFromLeft(img);
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
    
    return Newimage;
}

SDL_Surface* CropRight(SDL_Surface *img)
{
    Uint8 r, g, b;
    SDL_Surface *NewImage;
    int min = CalcMinFromRight(img);
    //int width = img->w;
    int height = img->h;
    NewImage = SDL_CreateRGBSurface(0, min, height, 32, 0, 0, 0, 0);
    SDL_FillRect(NewImage, NULL, SDL_MapRGB(NewImage->format, 255, 255, 255));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < min; j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            pixel = SDL_MapRGB(NewImage->format, r, g, b);
            put_pixel(NewImage, j, i, pixel);
        }
        
    
    }
    return NewImage;
    
}

SDL_Surface* CropTop(SDL_Surface *img)
{
    Uint8 r, g, b;
    SDL_Surface *NewImage;
    int min = CalcMinFromTop(img);
    int width = img->w;
    int height = img->h;
    height -= min;
    NewImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_FillRect(NewImage, NULL, SDL_MapRGB(NewImage->format, 255, 255, 255));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint32 pixel = get_pixel(img, j, i+min);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            pixel = SDL_MapRGB(NewImage->format, r, g, b);
            put_pixel(NewImage, j, i, pixel);
        }
        
    }
    return NewImage;
}


SDL_Surface* CropBottom(SDL_Surface *img)
{
    Uint8 r, g, b;
    SDL_Surface *NewImage;
    int min = CalcMinFromBot(img);
    int width = img->w;
    NewImage = SDL_CreateRGBSurface(0, width, min, 32, 0, 0, 0, 0);
    SDL_FillRect(NewImage, NULL, SDL_MapRGB(NewImage->format, 255, 255, 255));

    for (int i = 0; i < min; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            pixel = SDL_MapRGB(NewImage->format, r, g, b);
            put_pixel(NewImage, j, i, pixel);
        }
        
    }
    return NewImage;
    
}

SDL_Surface* ChipTheEdges(SDL_Surface *img)
{
    SDL_Surface* s;

    s = CropLeft(img);
    s = CropRight(s);
    s = CropTop(s);
    s = CropBottom(s);

    SDL_SaveBMP(s, "Images/CroppedImage.bmp");
    return s;

}




