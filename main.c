
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<err.h>
#include"text_extract.h"

//Initialize the SDL
void init_SDL()
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s. \n", SDL_GetError());
    
}



//Load the image in the memory
SDL_Surface* load_image(char* path)
{
    SDL_Surface *img;

    img = IMG_Load(path);
    if(!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
    return img;
}


//Display the image
SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    screen = SDL_SetVideoMode(img->w, img->h, img->format->BitsPerPixel, SDL_ANYFORMAT);

    if (!screen)
    {
        errx(1, "Couldn't set %dx%d video mode: %s\n",
            img->w, img->h, SDL_GetError());
    }
    return screen;
}










int main(int argc, char **argv)
{
   

    //the path  of the image we want to display
    char* imagePath = argv[1];
    bool displaying = true;
    SDL_Event _event ;
    SDL_Surface *screen, *image;
    SDL_Surface *s;
    //int** array;

    
    
    
    
    
    if (argc == 1)
    {
        errx(1, "Error: No path provided");
        return 1; 
    }
       
        
    init_SDL();
    image = load_image(imagePath);
    //int min = text_extract(image);
    //printf("%u", min);


    s = ImageFormation(image);

    screen = display_image(s);



    //A loop to keep on displaying till the time a key is not pressed

    while ((SDL_WaitEvent(&_event) != 0) && (displaying))
    {
        switch (_event.type)
        {
            case SDL_KEYUP:
                displaying = false;
                break;
            
            default:
                SDL_BlitSurface(s, 0, screen, 0);
                SDL_UpdateRect(screen, 0, 0, s->w, s->h);
                break;
        }
    }

    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(s);

    return 0;
    
}