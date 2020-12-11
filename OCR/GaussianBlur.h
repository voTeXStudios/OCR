#ifndef _GAUSSIAN_BLUR_
#define _GAUSSIAN_BLUR_

#include<stdlib.h>
#include"SDL/SDL.h"
void Convolution(SDL_Surface *img,double kernel[9]);
double gauss[9];
double sharpen[9];
double edge_dectection[9];

#endif
