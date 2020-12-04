#ifndef CROP_IMAGE
#define CROP_IMAGE

#include<stdlib.h>
#include"SDL/SDL.h"

//int CalcMinFromLeft(SDL_Surface *image);
//int CalcMinFromRight(SDL_Surface *image);
//int CalcMinFromTop(SDL_Surface *image);
//int CalcMinFromBot(SDL_Surface *image);
//SDL_Surface* CropLeft(SDL_Surface *img);
//SDL_Surface* CropRight(SDL_Surface *img);
//SDL_Surface* CropTop(SDL_Surface *img);
//SDL_Surface* CropBottom(SDL_Surface *img);
SDL_Surface* ChipTheEdges(SDL_Surface *img);


#endif
