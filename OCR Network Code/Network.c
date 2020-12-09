#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include "Layer.c"



struct Network
{
  Layer layers[3];
  double *output;
};

//Declarations
typedef struct Network Network;
double* FeedForwardXOR(double* input, Network net);

Network GenerateNetwork()
{
  double nb_neurons[3] = {200, 100, 26};
  double nb_inputs[3] = {784, 200, 100};
  Network net;
  for (size_t i = 0; i < 3; i++)
  {
    net.layers[i] = GenerateLayer(nb_neurons[i], nb_inputs[i]);
  }

  return net;
}

void BackPropagationXOR(Network net, double target, double inputs[])
{
  double error;
  double dl_dw;
  Neuron neuron;
  //double dl_daL_1;
  //Layer layer = net.layers[];
  int k = 1;
  while (k >= 0)
  {
    for (int i = 0; i < net.layers[k].nb_outputs; i++)
    {
      neuron = net.layers[k].neurons[i];
      //printf("Neuron Visited %i\n", i);
      for (int j = 0; j < neuron.nb_weights; j++)
      {
        //printf("k: %i\n", k);
        if (k == 1)
        {
          error = -(target - neuron.output); 

          dl_dw = error * sigmoid_derivative(neuron.output) * net.layers[k - 1].neurons[j].output;

          net.layers[k - 1].neurons[j].dl_wrt_curr += neuron.weights[j] * sigmoid_derivative(neuron.output) * error;
          //printf("dl_wrt_prevneuron: %f\n", net.layers[k-1].neurons[j].dl_wrt_curr);

          neuron.weights[j] = neuron.weights[j] - 0.6 * dl_dw;
          //printf("%f\n", neuron -> weights[j]);
        }
        else
        {
         // printf("%f\n", net.layers[k].neurons[j].dl_wrt_curr);
          dl_dw = net.layers[k].neurons[j].dl_wrt_curr * sigmoid_derivative(neuron.output) * inputs[j];

          neuron.weights[j] = neuron.weights[j] - 0.6 * dl_dw;
         // printf("%f\n", neuron -> weights[j]);
        }
      }
    }
    k--;
  }
  
}


void BackPropagation(Network net, double target[], double inputs[])
{
  double error;
  double dl_dw;
  Neuron neuron;

  int k = 2;
  while (k >= 0)
  {
    for (int i = 0; i < net.layers[k].nb_outputs; i++)
    {
      neuron = net.layers[k].neurons[i];
      for (int j = 0; j < neuron.nb_weights; j++)
      {
        // If iterating over last layer
        if (k == 2)
        {
          error = -(target[i] - neuron.output);

          dl_dw = error * sigmoid_derivative(neuron.output) * net.layers[k - 1].neurons[j].output;

          net.layers[k - 1].neurons[j].dl_wrt_curr += neuron.weights[j] * sigmoid_derivative(neuron.output) * error;

          neuron.weights[j] = neuron.weights[j] - 0.6 * dl_dw;
        }
        else if (k > 0)
        {
          dl_dw = neuron.dl_wrt_curr * sigmoid_derivative(neuron.output) * net.layers[k - 1].neurons[j].output;

          net.layers[k - 1].neurons[j].dl_wrt_curr += neuron.weights[j] * sigmoid_derivative(neuron.output) * neuron.dl_wrt_curr;

          neuron.weights[j] = neuron.weights[j] - 0.6 * dl_dw;
        }
        else
        {
          dl_dw = neuron.dl_wrt_curr * sigmoid_derivative(neuron.output) * inputs[j];

          neuron.weights[j] = neuron.weights[j] - 0.6 * dl_dw;
        }
      }
      
    }
    k--;
     
  }
  
}

void Train_model(double inp[], double* inputs, double target[], Network net, size_t iterations)
{
  double *res;
  for (size_t i = 0; i < iterations; i++)
  {
    res = FeedForward(inputs, net);
    BackPropagation(net, target, inp);

  }
  
}


void Train_modelXOR(double inp[], double* inputs, double target, Network net, double iterations)
{
  double *res;

  for (size_t i = 0; i < iterations; i++)
  {
   
    res = FeedForwardXOR(inputs, net);
    //printf("%f\n", *res);
    BackPropagationXOR(net, target, inp);
  }
  
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

double* FeedForward(double* input, Network net)
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

double* FeedForwardXOR(double* input, Network net)
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

void Prediction(double* result)
{
  if (*result>0.5)
    printf("The answer is %f\n", 1.0);
    
  else
    printf("The answer is %f\n", 0.0);
}

int main()
{
  srand(time(NULL));
  double arr[2] = {0, 0};
  double *inputs = arr;
  double target[26] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0};

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
  Network OCR = GenerateNetwork();

  /*for (size_t k = 0; k < 2 ; k++)
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
  }*/

  double* result;
  result = FeedForwardXOR(inputs, XOR);
  printf("Before Training, output is = %f\n", *result);
  printf("..............\n");

  Train_modelXOR(arr, inputs, 0, XOR, 10000);

  result = FeedForwardXOR(inputs, XOR);

  printf("After Training, output is = %f\n", *result);
  //Prediction(result);
}
