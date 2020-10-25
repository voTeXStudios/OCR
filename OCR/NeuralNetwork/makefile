#Start

all: Network

Network: neuronX.o
	gcc networkX.c -lm -o Network

neuronX.o: neuronX.c
	gcc -c networkX.c -lm

clean:
	${RM} *.o
	${RM} Network





#END


