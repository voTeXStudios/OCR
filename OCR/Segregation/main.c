
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<err.h>
#include"CropImage.h"
#include"CharDetection.h"
#include"BlackAndWhite.h"
#include"pixeloperations.h"
#include"LetterSegregate.h"

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
SDL_Surface* displayImage(SDL_Surface *img)
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










int main(int argc, char **argv)
{
   

    //the path  of the image we want to display
    char* imagePath = argv[1];
    //bool displaying = true;
    //SDL_Event _event ;
    SDL_Surface *screen, *image, *s;

    
    
    
    
    if (argc == 1)
    {
        errx(1, "Error: No path provided");
        return 1; 
    }
       
        
    init_SDL();
    image = load_image(imagePath);

    Binarise(image);
    s = ChipTheEdges(image);
    DetectCharacter(s);
    //Vertical_segmentation(s);

    Boundry(image);
    screen = displayImage(image);
    wait_for_keypressed();
    SDL_FreeSurface(image);
    
    
    
    screen = displayImage(s);
    wait_for_keypressed();
    



    //A loop to keep on displaying till the time a key is not pressed

    /*while ((SDL_WaitEvent(&_event) != 0) && (displaying))
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
    }*/

    SDL_FreeSurface(s);
    SDL_FreeSurface(screen);
    
    
   

    return 0;
    
}