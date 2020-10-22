
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int main(int argc, char **argv)
{
    //the path  of the image we want to display
    char* imagePath = argv[1];

    //bool to know if we should
    bool displaying = true;

    SDL_Event _event ;

    SDL_Surface *screen, *image;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("Error");
        return 1;
    }

    image = IMG_Load("test.bmp");
    printf("Imageloaded");
    if (!image)
    {
        printf("IMG_Load: Error");
        SDL_Quit();
        return 1;
    }
    
    screen = SDL_SetVideoMode(image->w, image->h, image->format->BitsPerPixel, SDL_ANYFORMAT);
    if (!screen)
    {
        printf("SDL_SetVideoMode: Error");
        SDL_FreeSurface(image);
        SDL_Quit();
        return 1;
    }

    while ((SDL_WaitEvent(&_event) != 0) && (displaying))
    {
        switch (_event.type)
        {
            case SDL_QUIT:
                displaying = false;
                break;
            
            default:
                SDL_BlitSurface(image, 0, screen, 0);
                SDL_UpdateRect(screen, 0, 0, image->w, image->h);
                break;
        }
    }

    SDL_FreeSurface(image);
    SDL_Quit();

    return 0;
    /*BMP* bmp;
    UCHAR r, g, b;
    UINT width, height;
    UINT x, y;

    if (argc != 3)
    {
        fprintf(stderr, "USAGE: %s <input file> <output file>\n", argv[0]);
        return 0;
    }

    bmp = BMP_ReadFile(argv[1]);
    //IMG_Load (argv[1]);
    BMP_CHECK_ERROR(stderr, -1);
    

    width = BMP_GetWidth(bmp);
    height = BMP_GetHeight(bmp);

    for(uint i = 0; i < width; i++)
    {
        for (uint j = 0; j < height; j++)
        {
            BMP_GetPixelRGB(bmp, i, j, &r, &g, &b);

            BMP_SetPixelRGB(bmp, i, j, 255 -  r, 255 - g, 255 - b);
        }
        
    }

    BMP_WriteFile(bmp, argv[2]);
    BMP_CHECK_ERROR(stderr, -2);

    BMP_Free(bmp);
    return 0;*/

    
    
}

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
           if (*r != 255 && *g != 255 && *b != 255)
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

    SDL_SaveBMP(dst_image, "res.bmp");

   
    return 0;
}