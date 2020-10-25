#ifndef LETTER_SEGREGATION_
#define LETTER_SEGREGATION_

#include<stdlib.h>
#include "SDL/SDL.h"

bool check (SDL_Surface *image);
void Vertical_segmentation (SDL_Surface *image);
void Image_Formation(SDL_Surface *img, int min, int max);

#endif