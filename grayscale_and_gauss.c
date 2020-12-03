#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pixel_operations.h"

// TODO: Insert all the above functions.

double gauss_kernel_d3[9] =    // guauss kernel dim 3 (approximation), calculated ahead of time
{
    1./16., 1./8., 1./16.,
    
    1./8., 1./4., 1./8.,
    
    1./16., 1./8., 1./16.
};

void init_sdl()
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
}


void convolute(SDL_Surface* image_surface, double m[], size_t cols){ 
       	//intended for grayscaled image and kernel of dim 3
    
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    double sum;
    Uint8 r1=0, g1=0, b1=0;
    Uint32 pixel;
    SDL_Surface *image_surface_copy = SDL_CreateRGBSurface(image_surface-> flags, width, height, image_surface->format->BitsPerPixel,image_surface->format->Rmask, image_surface->format->Gmask, image_surface->format->Bmask, image_surface->format->Amask);
    SDL_BlitSurface(image_surface, NULL, image_surface_copy,NULL);
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
                    pixel = get_pixel(image_surface_copy,y+j,x+i);
                    SDL_GetRGB(pixel, image_surface_copy->format, &r1, &g1, &b1);
                    sum += (r1 * m[(x+1)*cols + y+1]);
                }
            }
	    pixel = SDL_MapRGB(image_surface->format, sum, sum, sum);
            put_pixel(image_surface,j,i,pixel);
        }
    }
    for (size_t i = 0; i < width; i++)
    {
        pixel = get_pixel(image_surface,i,0);
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        pixel = get_pixel(image_surface,i,height-1);
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255)
    }
    
    for (size_t i = 0; i < height; i++)
    {
        pixel = get_pixel(image_surface,0,i);
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        pixel = get_pixel(image_surface,width-1,i);
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255)
    }
    //possibility to crop image, with white contour this won't matter though
    
}

int main()
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
}
