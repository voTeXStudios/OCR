#Start

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main Network

main: CropImage.o CharDetection.o BlackAndWhite.o pixeloperations.o GaussianBlur.o

Network: neuronX.o
	gcc networkX.c -lm -o Network

neuronX.o: neuronX.c
	gcc -c networkX.c -lm


CropImage.o: CropImage.c CropImage.h pixeloperations.h

#DetectTextBlock.o: DetectTextBlock.c pixeloperations.h CropImage.h DetectTextBlock.h

CharDetection.o: CharDetection.c pixeloperations.h CharDetection.h

BlackAndWhite.o: BlackAndWhite.c pixeloperations.h BlackAndWhite.h

GaussianBlur.o: pixeloperations.h GaussianBlur.c GaussianBlur.h

pixeloperations.o: pixeloperations.c pixeloperations.h



#LetterSegregate.o: LetterSegregate.c LetterSegregate.h pixeloperations.h BlackAndWhite.h

#LineSegregation.o: LineSegregation.c LineSegregation.h CropImage.h pixeloperations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} Network
	${RM} *.bmp

#END
	
