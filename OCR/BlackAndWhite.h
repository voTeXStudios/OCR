#ifndef BLACK_WHITE_H
#define BLACK_WHITE_H

#include<stdlib.h>
#include"SDL/SDL.h"

void grayScale();
void binarise(SDL_Surface* image_surface, float limit);

#endif