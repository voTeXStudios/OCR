#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include<stdbool.h>
#include "pixeloperations.h"
#include "GaussianBlur.h"


double gauss_kernel_d3[9] =    // guauss kernel dim 3 (approximation), calculated ahead of time
{
    1./16., 1./8., 1./16.,
    
    1./8., 1./4., 1./8.,
    
    1./16., 1./8., 1./16.
};



void convolute(SDL_Surface* image_surface, double m[], size_t cols){ 
       	//intended for grayscaled image and kernel of dim 3
    
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    double sum;

    Uint8 r, g, b;
    Uint32 pixel;
    for (size_t i = 1; i < height-1; i++)
    {
        for (size_t j = 1; j < width-1; j++)
        {
            sum = 0;
            for (int x = -1; x <= 1; x++)
            {
		   // printf("%ld",x);
                for (int y = -1; y <= 1; y++)
                {
                    pixel = get_pixel(image_surface,y+j,x+i);
                    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                    sum += (r * m[(x+1)*cols + y+1]);
                }
            }
	        pixel = SDL_MapRGB(image_surface->format, sum, sum, sum);
            put_pixel(image_surface,j,i,pixel);
        }
    }

    for (size_t i = 0; i < width; i++)
    {
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        put_pixel(image_surface, i, 0, pixel);
        put_pixel(image_surface, i, height-1, pixel);
    }

    for (size_t i = 0; i < height; i++)
    {
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        put_pixel(image_surface, 0, i, pixel);
        put_pixel(image_surface, width - 1, i, pixel);
    
    }

    
    
    //possibility to crop image, with white contour this won't matter though
    
}

void Convolution(SDL_Surface *img)
{
   // SDL_Surface* s;
    convolute(img, gauss_kernel_d3, 3);
    //return s;
}
