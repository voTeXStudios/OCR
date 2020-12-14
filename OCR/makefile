#Start

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl --cflags gtk+-3.0` -MMD -Wall -O3
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl --libs gtk+-3.0` -lSDL_image -lm -lSDL
all: UI_code

UI_code: CropImage.o CharDetection.o BlackAndWhite.o pixeloperations.o GaussianBlur.o Network.o Layer.o Neuron.o UI_code.o

CropImage.o: CropImage.c CropImage.h pixeloperations.h

CharDetection.o: CharDetection.c pixeloperations.h CharDetection.h

BlackAndWhite.o: BlackAndWhite.c pixeloperations.h BlackAndWhite.h

GaussianBlur.o: pixeloperations.h GaussianBlur.c GaussianBlur.h

pixeloperations.o: pixeloperations.c pixeloperations.h

Network.o: Network.c Network.h Layer.h Neuron.h pixeloperations.h

Layer.o: Layer.c Layer.h

Neuron.o: Neuron.c

UI_code.o : UI_code.c

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
