#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>

struct Neuron
{
	double weights[2];
	//double bias;
	double output;
};

typedef struct Neuron Neuron;

double sigmoid(double x)
{
     double result;
     result = 1 / (1 + exp(-x));
     return result;
}

/*This function applies the basic calculations involved in a neural network to a specifc node
and returns an output of 'double' data type.*/

double ProcessNeuron(double inputs[], Neuron * N)
{
	N -> output = 0;
  for(int i = 0; i < 2; i++)
	{
    N -> output += N -> weights[i] * inputs[i];
	}
	//N.output += N.bias;
	N -> output = sigmoid(N -> output);
	return N -> output;
}

double randomnum()
{
  return (double)rand() / (double)RAND_MAX;
}

Neuron GenerateNeuron()
{
	Neuron n;
	n.output = 0;
	n.weights[0] = randomnum();
	n.weights[1] = randomnum();
	//n.bias = rand() % 21;
	return n;
}
