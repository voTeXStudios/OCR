#include"SDL/SDL.h"
#include"SDL/SDL_image.h"
#include<stdio.h>
#include<stdlib.h>
#include "pixeloperations.h"
#include "CropImage.h"
#include<err.h>

// A potential Image bug fix
SDL_Surface* IncreaseTheSizeImage(SDL_Surface* image)
{
    Uint8 r, g, b;
    Uint32 pixel;
    SDL_Surface* newImage;
    int height = image->h;
    int width = image->w;
    int new_Width = width + 10;
    int new_height = height + 10;
    newImage = SDL_CreateRGBSurface(0, new_Width, new_height, 32, 0, 0, 0, 0);
    SDL_FillRect(newImage, NULL, SDL_MapRGB(newImage->format, 255, 255, 255));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel = get_pixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            pixel = SDL_MapRGB(newImage->format, r, g, b);
            put_pixel(newImage, j+5, i+5, pixel);
        }

    }
    return newImage;
}


/*int CalcMinFromLeft(SDL_Surface *image)
{

   Uint8 r, g, b;
   Uint32 pixel;
   int min = INT32_MAX;
   int width = image->w;
   int height = image->h;
   for (int i = 0; i < height; i++)
   {
       for (int j = 0; j < width; j++)
       {
           pixel = get_pixel(image, j, i);
           SDL_GetRGB(pixel, image->format, &r, &g, &b);
           if (r == 0 && g == 0 && b == 0)
           {
               if (j < min)
               {
                    if (j - 3 >= 0)
                        min = j - 3;
                    else
                        min = j;

                    break;
               }


           }

       }

    }
    return min;

}

int CalcMinFromRight(SDL_Surface *image)
{
    Uint8 r, g, b;
    Uint32 pixel;
    int min = INT32_MIN;
    int width = image->w;
    int height = image->h;
    for (int i = 0; i < height; i++)
    {
        for (int j = width - 1; j >= 0; j--)
        {
            pixel = get_pixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r == 0 && g == 0 && b  == 0)
            {
                if (j>min)
                {
                    if(j + 3 < width)
                        min = j + 3;

                    else
                        min = j;

                    break;
                }
            }
        }

    }
    return min;
}

int CalcMinFromTop(SDL_Surface *image)
{
	Uint8 r, g, b;
    Uint32 pixel;
	int min = INT32_MAX;
	int width = image->w;
	int height = image->h;
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j <  height; j++)
		{
			pixel = get_pixel(image, i, j);
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			if (r == 0 && g == 0 && b == 0)
			{
				if (j < min)
				{
                    if (j - 3 >= 0)
                        min = j - 3;

                    else
                        min = j;

                    break;
				}
			}
		}
	}
  return min;
}

int CalcMinFromBot(SDL_Surface *image)
{
	Uint8 r, g, b;
    Uint32 pixel;
	int min = INT32_MIN;
	int width = image->w;
	int height = image->h;
	for(int i = 0; i < width; i++)
	{
		for(int j = height-1; j >= 0; j--)
		{
			pixel = get_pixel(image, i, j);
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			if (r == 0 && g == 0 && b == 0)
			{
				if (j > min)
				{
                    if (j + 3 < height)
                        min = j + 3;   

                    else
                        min = j;

                    break;
				}
			}
		}
	}
    return min;
}




SDL_Surface* Crop(SDL_Surface* img)
{
    Uint8 r, g, b;
    Uint32 pixel;
    SDL_Surface* newImage;
    int mintop = CalcMinFromTop(img);
    int minBot = CalcMinFromBot(img);
    int minLeft = CalcMinFromLeft(img);
    int minRight = CalcMinFromRight(img);
    int newImageX;
    int newImageY = 0;
    newImage = SDL_CreateRGBSurface(0, minRight-minLeft, minBot-mintop, 32, 0, 0, 0, 0);
    SDL_FillRect(newImage, NULL, SDL_MapRGB(newImage->format, 255, 255, 255));


    for (int i = mintop; i < minBot; i++)
    {
        newImageX = 0;
        for (int j = minLeft; j < minRight; j++)
        {
            pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            pixel = SDL_MapRGB(newImage->format, r, g, b);
            put_pixel(newImage, newImageX, newImageY, pixel);
            newImageX += 1;
        }
        newImageY += 1;
    }
    return newImage;
}*/





SDL_Surface* ChipTheEdges(SDL_Surface *img)
{
    SDL_Surface* s;
    //s = Crop(img);
    s = IncreaseTheSizeImage(img);


   // SDL_SaveBMP(s, "Images/CroppedImage.bmp");
    return s;

}
