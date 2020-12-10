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
/*
double gauss[9] = 
{
    1./9., 2./9., 1./9.,
    2./9., 4./9., 2./9.,
    1./9., 2./9., 1./9.

};


int Clamp(float c)
{
    if (c > 255)
        return 255;
    else if (c < 0)
        return 0;
    else
        return (int)c;
}

bool IsValid(int x, int y, int height, int width)
{
    if (x >= 0 && x < width && y >=0 && y < height)
        return true;
    else
        return false;
}

void Convolute(SDL_Surface *image_surface, double m[], int cols)
{
    int width = image_surface->w;
    int height = image_surface->h;
    int offset = (int)cols/2;
    Uint32 pixel;
    Uint8 r, g, b, r1, g1, b1;
    SDL_Surface *image_surface_copy = SDL_CreateRGBSurface(image_surface-> flags, width, height, image_surface->format->BitsPerPixel,image_surface->format->Rmask, image_surface->format->Gmask, image_surface->format->Bmask, image_surface->format->Amask);
    SDL_BlitSurface(image_surface, NULL, image_surface_copy,NULL);
    float coefficient;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
           r1 = g1 = b1 = 0;

           for (int dy = -offset; dy <= offset; dy++)
           {
               for (int dx = -offset; dx <= offset; dx++)
               {
                   if (IsValid(x + dx, y + dy, height, width))
                   {
                       pixel = get_pixel(image_surface_copy, x + dx, y + dy);
                       SDL_GetRGB(pixel, image_surface_copy->format, &r, &g, &b);
                       coefficient = m[(dy + offset) * cols + (dx + offset)];

                       r1 += r * coefficient;
                       g1 += g * coefficient;
                       b1 += b * coefficient;
                   }
               }
           }
           r1 = Clamp(r1);
           g1 = Clamp(g1);
           b1 = Clamp(b1);
           pixel = SDL_MapRGB(image_surface->format, r1, g1, b1);
           put_pixel(image_surface, x, y, pixel);
        }
    }
    //return image_surface_copy;


}*/


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
