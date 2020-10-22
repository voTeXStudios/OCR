#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}


int text_extract(SDL_Surface *image)
{
   int *r;
   int *g;
   int *b;
   int min = INT32_MAX;
   int width = image->w;
   int height = image->h;
   for (int i = 0; i < height; i++)
   {
       for (int j = 0; j < width; j++)
       {
           SDL_LockSurface(image);
           Uint32 pixel = getpixel(image, i, j);
           SDL_UnlockSurface(image);
           SDL_GetRGB(pixel, image->format, *r, *g, *b);
           int a = *r;
           int d = *g; 
           int c = *b;
           if (a != 255 && d != 255 && c != 255)
           {
               if (j < min)
               {
                   min = j;
               }
           }
           
       }
       
    }

    SDL_Rect *srcrect;
    srcrect->x = min;
    srcrect->y = 0;
    srcrect->h = height;
    srcrect->w = width - min;

    SDL_Rect *dstrect;
    dstrect->x = 0;
    dstrect->y = 0;
    dstrect->h = height;
    dstrect->w = srcrect->w;

    SDL_Surface *dst_image;

    SDL_BlitSurface(image,srcrect,dst_image,dstrect);

    SDL_SaveBMP(dst_image, "/home/sofiane/Documents/OCR PROJECT/OCR/");

   

}