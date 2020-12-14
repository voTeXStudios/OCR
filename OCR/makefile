#Start

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lSDL_gfx -lm
all: main Network

main: CropImage.o CharDetection.o BlackAndWhite.o pixeloperations.o GaussianBlur.o deskew.o

Network: Layer.o Neuron.o
	gcc Network.c -lm -lSDL -lSDL_image -o Network

#neuronX.o: neuronX.c
	#gcc -c networkX.c -lm

neuronX.o: Layer.c Neuron.c
	gcc -c Network.c -lm -lSDL -lSDL_image


CropImage.o: CropImage.c CropImage.h pixeloperations.h

#DetectTextBlock.o: DetectTextBlock.c pixeloperations.h CropImage.h DetectTextBlock.h

CharDetection.o: CharDetection.c pixeloperations.h CharDetection.h

BlackAndWhite.o: BlackAndWhite.c pixeloperations.h BlackAndWhite.h

GaussianBlur.o: pixeloperations.h GaussianBlur.c GaussianBlur.h

pixeloperations.o: pixeloperations.c pixeloperations.h

deskew.o : deskew.c deskew.h

#LetterSegregate.o: LetterSegregate.c LetterSegregate.h pixeloperations.h BlackAndWhite.h

#LineSegregation.o: LineSegregation.c LineSegregation.h CropImage.h pixeloperations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} Network
	${RM} *.bmp
	#${RM} batch[1-2]_layer[1-3]data
	${RM} layer[1-3]data
	${RM} Network data.txt

#END
