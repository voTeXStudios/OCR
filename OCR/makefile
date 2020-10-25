#Start

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main Network

main: CropImage.o DetectTextBlock.o CharDetection.o BlackAndWhite.o pixeloperations.o LetterSegregate.o

Network: neuronX.o
	gcc networkX.c -lm -o Network

neuronX.o: neuronX.c
	gcc -c networkX.c -lm


CropImage.o: CropImage.c CropImage.h pixeloperations.h
	gcc -c CropImage.c

DetectTextBlock.o: DetectTextBlock.c pixeloperations.h CropImage.h DetectTextBlock.h
	gcc -c DetectTextBlock.c

CharDetection.o: CharDetection.c pixeloperations.h CropImage.h CharDetection.h
	gcc -c CharDetection.c

BlackAndWhite.o: BlackAndWhite.c pixeloperations.h BlackAndWhite.h
	gcc -c BlackAndWhite.c

pixeloperations.o: pixeloperations.c pixeloperations.h
	gcc -c pixeloperations.c

LetterSegregate.o: LetterSegregate.c LetterSegregate.h pixeloperations.h BlackAndWhite.h
	gcc -c LetterSegregate.c


clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} Network

#END
	
