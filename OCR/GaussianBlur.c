#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<err.h>
#include<math.h>
#include "pixeloperations.h"



//program to generate Gaussian filter

double** FilterCreation()
{
    double sigma = 1.0; //here we do it for signma =1
    double r,s = 2.0 * sigma * sigma;

    double sum = 0.0;
    double GKernel[3][3] = {{0}};

for (int x = -1; x <= 1; x++) { 
        for (int y = -1; y <= 1; y++) { 
            r = sqrt(x * x + y * y); 
            GKernel[x + 1][y + 1] = (exp(-(r * r) / s)) / (M_PI * s); 
            sum += GKernel[x + 1][y + 1]; 
        } 
    } 

    // normalising the Kernel 
    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 3; ++j) 
            GKernel[i][j] /= sum; 


return GKernel;
}


// for edge cases we will ignore, though there are different solutions to this problem, it's unlikely that the letters will be
// on the edge of the image.

// The Gaussian smoothing can be performed using standard convolution methods now that we can produce the kernel

void gaussian_blurring(SDL_Surface* image_surface, double kernal[3][3]){  //intended for grayscaled image

    int width = image_surface->w;
    int height = image_surface->h;
    FilterCreation();
    double sum = 0;
    Uint8 r1, g1, b1;
    Uint32 pixel;
    SDL_Surface* image_surface_copy = image_surface;
    
    for (size_t i = 0; i < height-2; i++)
    {
        for (size_t j = 0; j < width-2; i++)
        {
            for (size_t x = -1; i <= 1; i++)
            {
                for (size_t y = -1; i <= 1; i++)
                {
                    pixel = get_pixel(image_surface_copy,j+y,x+i);
                    DL_GetRGB(pixel, image_surface_copy->format, &r1, &g1, &b1);
                    sum += r1 * kernal[y+1][x+1];
                }
                
            }

            pixel = SDL_MapRGB(image_surface->format, r1, g1, b1);
            put_pixel(image_surface,j,i,pixel);


        }
        
    }




    //use kernel crop






}
