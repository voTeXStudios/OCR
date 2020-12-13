#include <stdio.h>
#include <stdlib.h>
#include<SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include"deskew.h"
#include <math.h>
#include"pixeloperations.h"
#define PRECISION 720


SDL_Surface *man_deskew(SDL_Surface* image_surface, double degree){

	return rotozoomSurface(image_surface, degree,(double)1,(int)0);
}



double variance(size_t arr[],size_t n)
{

	int sum = 0;
	int sum1 = 0;
	for (size_t i = 0; i < n; i++)
    {
        sum = sum + arr[i];
    }
    double average = sum / (float)n;

    for (size_t i = 0; i < n; i++)
    {
        sum1 = sum1 + pow((arr[i] - average), 2);
    }

    return (sum1 / (float)n);
}

double skew_deg(SDL_Surface* image){
	size_t h = image->h;
	size_t w = image->w;
	size_t hist[h];
	size_t sum;
	Uint8 r,g,b;
	double max_var = 0;
	double max_var_deg=0;
	double var;
	Uint32 pixel;
	for(size_t l=0; l<(size_t)PRECISION; l++){
		man_deskew(image,(double)l/2.0);
		for (size_t i=0; i<h;i++){
			sum = 0;
				for(size_t j=0; j<w; j++){
					pixel = get_pixel(image,j,i);
					SDL_GetRGB(pixel, image->format, &r, &g, &b);
					if (r== 0)
						sum +=1;
				}
			hist[i]= sum;
		}
		var = variance(hist, h);
		if (var >max_var){
			max_var = var;
			max_var_deg = l *0.5;
		}
	}
	return max_var_deg;
}

SDL_Surface *auto_deskew(SDL_Surface* img)
{
return man_deskew(img, skew_deg(img));

}
