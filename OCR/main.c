
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<err.h>
#include"CropImage.h"
#include"CharDetection.h"
#include"BlackAndWhite.h"
#include"pixeloperations.h"
#include"GaussianBlur.h"
#include"deskew.h"

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






// Saves the images. This function is Temporary //
void SaveImages(SDL_Surface** surfaces)
{
    int i = 0;
    while (*(surfaces + i) != NULL)
    {
        char number_str[100];
        sprintf(number_str, "%d", i);
        strcat(number_str, "char.bmp");
        SDL_SaveBMP(*(surfaces + i), number_str);
        i++;
        //train_images(*(surfaces + i)); 
    }
}








int main(int argc, char **argv)
{
   
    if (argc != 2)
    {
        errx(1, "One Image required");
        return 1;
    }

    char* imagePath = argv[1];
    SDL_Surface *screen;
    SDL_Surface *image;
    //SDL_Surface *s;
    //SDL_Surface **surfaces;
    //Initialise SDL.    
    init_SDL();

    //Load the required image.
    image = load_image(imagePath);
    if (!image)
        errx(1, "null surface was given, try again with appropriate image");
   /* 
    if (image->h > 2000 && image->w > 2000)
    {
        image = Compression(image, 1000, 1000);
        printf("Image Compressed to 1000*1000\n");
    }
    */
    screen = displayImage(image);

    wait_for_keypressed();
    //Contrast(image,40);
    man_deskew(image,90.0);
    // pick a value from -100 to 100 from decontrasted to ++constrast
    update_surface(screen, image);

    
    wait_for_keypressed();
   Convolution(image,gauss);
   update_surface(screen, image);
    wait_for_keypressed();
    grayScale(image);
   update_surface(screen, image);
   // wait_for_keypressed();



     //update_surface(screen, image);
  // wait_for_keypressed();
      // Convolution(image,gauss);
      // update_surface(screen, image);
        //wait_for_keypressed();
        //Convolution(image,edge_detection);
        //update_surface(screen, image);

    
    //Convolution(image,sharpen);
    //update_surface(screen, image);
    //wait_for_keypressed();

       Binarise(image);
    update_surface(screen, image);
    wait_for_keypressed();
    


    //Crop the unwanted portion of the image.
    //s = ChipTheEdges(image);

    update_surface(screen, image);
    wait_for_keypressed();


    //Detect the characters.
    //surfaces = DetectCharacter(s);

    //screen = displayImage(s);


    //Save Images in files
    //SaveImages(surfaces);

    
    

    //printf("%i", NbCharacters());

  


   
    wait_for_keypressed();
    
   
    

    
    
    
   
    //free(surfaces);   
    SDL_FreeSurface(screen);
    //SDL_FreeSurface(s);
    SDL_FreeSurface(image);
  

    

    return 0;
    
}
