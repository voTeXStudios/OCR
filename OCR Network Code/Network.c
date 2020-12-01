#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include "Layer.c"

struct Network
{
  Layer layers[4];
  double *output;
};

typedef struct Network Network;

Network GenerateNetwork()
{
  double nb_neurons[4] = {549, 392, 287, 28};
  double nb_inputs[4] = {784, 549,392, 287};
  Network net;
  for (size_t i = 0; i < 4; i++)
  {
    net.layers[i] = GenerateLayer(nb_neurons[i], nb_inputs[i]);
  }

  return net;
}

Network GenerateNetworkXOR()
{
  double nb_neurons[2] = {2, 1};
  double nb_inputs[2] = {2, 2};
  Network net;
  for (size_t i = 0; i < 2; i++)
  {
    net.layers[i] = GenerateLayer(nb_neurons[i], nb_inputs[i]);
  }

  return net;
}

double * FeedForward(double *input, Network net)
{
  double nb_inputs = 784;
  net.output = input;
  for(size_t i = 0; i < 4; i++)
  {
    net.output = ProcessLayer(net.output, &net.layers[i], nb_inputs);
    nb_inputs = net.layers[i].nb_outputs;
  }
  return net.output;
}

double * FeedForwardXOR(double *input, Network net)
{
  double nb_inputs = 2;
  net.output = input;
  for(size_t i = 0; i < 2; i++)
  {
    net.output = ProcessLayer(net.output, &net.layers[i], nb_inputs);
    nb_inputs = net.layers[i].nb_outputs;
  }
  return net.output;
}

int main()
{
  srand(time(NULL));
  double arr[2] = {1, 0};
  double *inputs = arr;

  /*Neuron n = GenerateNeuron(2);
  for (size_t i = 0; i < n.nb_weights; i++)
  {
    printf(" weight%li = %f\n",i , n.weights[i]);
  }
  printf(" bias = %f\n", n.bias);

  ProcessNeuron(inputs, &n);

  printf(" %f\n", n.output);
  Layer l = GenerateLayer(2, 2);
  for (size_t i = 0; i < l.nb_outputs; i++)
  {
    printf(" neuron %ld\n", i);
    for (size_t j = 0; j < l.neurons -> nb_weights; j++)
    {
      printf(" weight%li = %f\n",j , (l.neurons[i].weights)[j]);
    }
    printf(" bias = %f\n", l.neurons[i].bias);
  }

  ProcessLayer(inputs, &l, 2);

  for (size_t i = 0; i < l.nb_outputs; i++)
  {
    printf(" output%li = %f\n",i , l.output[i]);
  }*/

  Network XOR = GenerateNetworkXOR();

  for (size_t k = 0; k < 4 ; k++)
  {
    printf("layer%ld\n", k);
    for (size_t i = 0; i < XOR.layers[k].nb_outputs; i++)
    {
      printf(" neuron %ld\n", i);
      for (size_t j = 0; j < XOR.layers[k].neurons -> nb_weights; j++)
      {
        printf(" weight%li = %f\n",j , (XOR.layers[k].neurons[i].weights)[j]);
      }
      printf(" bias = %f\n", XOR.layers[k].neurons[i].bias);
    }
  }

  double * result = FeedForwardXOR(inputs, XOR);

  printf("output is = %f\n", *result);
}
