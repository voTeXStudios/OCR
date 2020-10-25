#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "pixeloperations.h"
#include<err.h>
#include "BlackAndWhite.h"
#include "GaussianBlur.h"


void grayscale(SDL_Surface* image_surface)
{

    int width = image_surface->w;
    int height = image_surface->h;
    Uint8 r, g, b, newcolor;
    Uint32 pixel;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            pixel = get_pixel(image_surface,x,y);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            newcolor = 0.3*r + 0.59*g + 0.11*b;
            r = g = b = newcolor;
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface,x,y,pixel);



        }
        
    }
}

float mean_value(SDL_Surface* image_surface)  // image here has been grayscaled
{

    int width = image_surface->w;
    int height = image_surface->h;
    Uint32 pixel;
    Uint8 r, g, b;
    int res = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            pixel = get_pixel(image_surface,x,y);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            res+= r;

            



        }
        
    }
    return res / (width * height);    //mean value
}



float automatic_threshold_value(SDL_Surface* image_surface, float limit)  // image here has been grayscaled
{

    float old_thre=0;
    float new_thre = mean_value(image_surface);
    int width = image_surface->w;
    int height = image_surface->h;
    Uint32 pixel;
    Uint8 r, g, b;
    float count1, count2, total1, total2;

    while(old_thre - new_thre >= limit || old_thre - new_thre <= -limit)
    {
        old_thre = new_thre;
        count1 = count2 = total1 = total2 = 0;
        

        for (int x = 0; x < width; x++){
            for (int y = 0; y < height; y++){

            pixel = get_pixel(image_surface,x,y);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if(r <= old_thre){
                count1++;
                total1 += r;
            }
            else{
                count2++;
                total2 += r;
                }
            }
        }
        new_thre = ((total1/count1)+(total2/count2))/2.;

    }
    return old_thre;
}

/*   The limit mentioned below is user definable. A larger limit will allow 
a greater difference between successive threshold values. 
Advantages of this can be quicker execution but with a less clear boundary 
between background and foreground. I found 10 is a pretty good value. anything below 1 is redundant (0.5 if rounding issue is fixed).
The lower the limit the more background noise is reduced but letters get more constricted. If image is not uniform in lighting or background
algorithm will still work but not as well.


I wanted to round numbers in the end but was having issues compiling therefore the threshold of 120.86 will truncate to 120 instead of
rounding up :l */

void binarization(SDL_Surface* image_surface, float limit)
{

    int width = image_surface->w;
    int height = image_surface->h;
    Uint8 r, g, b;
    Uint32 pixel;
    grayscale(image_surface);
    gaussian_blurring(image_surface,FilterCreation);

    int threshold = automatic_threshold_value(image_surface,limit);
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            pixel = get_pixel(image_surface,x,y);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if(r < threshold){
                r = g = b = 0;
            }

            else{
                r = g = b = 255;
            }
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface,x,y,pixel);


        }
        
    }



}


