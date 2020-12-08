#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include<stdbool.h>
#include "pixeloperations.h"
#include "GaussianBlur.h"

// TODO: Insert all the above functions.

double gauss_kernel_d3[9] =    // guauss kernel dim 3 (approximation), calculated ahead of time
{
    1./16., 1./8., 1./16.,
    
    1./8., 1./4., 1./8.,
    
    1./16., 1./8., 1./16.
};

double gauss[9] = 
{
    1./9., 2./9., 1./9.,
    2./9., 4./9., 2./9.,
    1./9., 2./9., 1./9.

};

/*void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

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
}*/

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

void convolute_tp(SDL_Surface *image_surface, double m[], int cols)
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


}


void convolute(SDL_Surface* image_surface, double m[], size_t cols){ 
       	//intended for grayscaled image and kernel of dim 3
    
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    double sum;

    Uint8 r,g,b;
    Uint32 pixel;
    SDL_Surface *image_surface_copy  = SDL_ConvertSurface(image_surface, image_surface->format, SDL_SWSURFACE);
    for (size_t i = 1; i < height-1; i++)
    {
        for (size_t j = 1; j < width-1; j++)
        {
            sum = 0;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    pixel = get_pixel(image_surface_copy,y+j,x+i);
		    SDL_GetRGB(pixel, image_surface_copy->format, &r, &g, &b);
		    sum += (r * m[(x+1)*cols + y+1]);
		   
                }
            }
	        pixel = SDL_MapRGB(image_surface->format, sum, sum, sum);
            put_pixel(image_surface,j,i,pixel);
        }
    }
    SDL_FreeSurface(image_surface_copy);


    for (size_t i = 0; i < width; i++)
    {
 
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
	put_pixel(image_surface,i,0,pixel);
        put_pixel(image_surface,i,height-1,pixel);
    }
    
    for (size_t i = 0; i < height; i++)
    {
	pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        put_pixel(image_surface,0,i,pixel);
        put_pixel(image_surface,width-1,i,pixel);
    }
    //possibility to crop image, with white contour. Done above^^.
    
}

void Convolution(SDL_Surface *img)
{
   // SDL_Surface* s;
    convolute(img, gauss_kernel_d3, 3);
    //return s;
}

/*int main()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // TODO: Initialize the SDL
    init_sdl();

    image_surface = load_image("my_image.jpg");
    // TODO: Display the image.
    screen_surface = display_image(image_surface);
	
    // TODO: Wait for a key to be pressed.
    wait_for_keypressed();

    grayscale(image_surface);

    update_surface(screen_surface,image_surface);
    wait_for_keypressed();
    size_t dim = 3;

    convolute(image_surface,gauss_kernel_d3,dim);
    update_surface(screen_surface,image_surface);
    wait_for_keypressed();

    // TODO: Free the image surface.
    SDL_FreeSurface(image_surface);
    // TODO: Free the screen surface.
    SDL_FreeSurface(screen_surface);





    return 0;
}*/
