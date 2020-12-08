
CC=gcc


all: Network

Network: Layer.o Neuron.o
	gcc Network.c -lm -o Network

neuronX.o: Layer.c Neuron.c
	gcc -c Network.c -lm
clean:
	${RM} *.o
	${RM} *.d
	${RM} Network


