#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include "neuronX.c"

/* This funtion is used to retrieve the information of a specific neuron.
   Mostly used for debugging*/

void printinfo(Neuron * N)
{
  printf("weight1 = %f, weight2 = %f\n", N -> weights[0], N -> weights[1]);
  printf("output for N = %f\n", N -> output);
}

/* This funtion is used to simulate the feed forward process in a network using only 3 neurons.
   Used to check outputs and to train*/

double PseudoFeedForward(Neuron * N1, Neuron * N2, Neuron * N3, double inputs[])
{
  double output1[2];
  double output2;
  output1[0] = ProcessNeuron(inputs, N1);
  output1[1] = ProcessNeuron(inputs, N2);
  output2 = ProcessNeuron(output1, N3);
  return output2;
}

/* This funtion is used to simulate the back propogation process in a network using only 3 neurons.
   Used to train*/

void PseudoBackProp(Neuron * N1, Neuron * N2, Neuron * N3, double guess, double target, double inputs[])
{
  double rateoflearning = 0.5;
  double errtot = target - guess;

  double outweight1 = N3 -> weights[0];
  double outweight2 = N3 -> weights[1];

  double weight1h1 = N1 -> weights[0];
  double weight2h1 = N1 -> weights[1];

  double weight1h2 = N2 -> weights[0];
  double weight2h2 = N2 -> weights[1];

  double deltaout = -errtot * (guess*(1 - guess));
  double deltah1 = deltaout * outweight1 * inputs[0] * (N1 -> output*(1 - N1 -> output));
  double deltah2 = deltaout * outweight2 * inputs[1] * (N2 -> output*(1 - N2 -> output));

  N3 -> weights[0] = outweight1 - (rateoflearning * deltaout);
  N3 -> weights[1] = outweight2 - (rateoflearning * deltaout);

  N1 -> weights[0] = weight1h1 - (rateoflearning * deltah1);
  N1 -> weights[1] = weight2h1 - (rateoflearning * deltah1);

  N2 -> weights[0] = weight1h2 - (rateoflearning * deltah2);
  N2 -> weights[1] = weight1h2 - (rateoflearning * deltah2);

}

/* This funtion is used to train the network of 3 neurons
   Used to change guesses of the network to desired outputs*/

void PseudoTrain(Neuron * N1, Neuron * N2, Neuron * N3, int iterations, double inputs[], double target)
{
  double res;
  for (int i = 0 ; i < iterations; i++)
  {
    res = PseudoFeedForward(N1, N2, N3, inputs);
    PseudoBackProp(N1, N2, N3, res, target, inputs);
    res = PseudoFeedForward(N1, N2, N3, inputs);
  }
}

/* This funtion is used to display the progress of the network after several interations of training.
   Used to check progress made by the network*/

void DisplayProgress(Neuron * N1, Neuron * N2, Neuron * N3, int iterations, double inputs[], double target)
{
  double res;
  res = PseudoFeedForward(N1, N2, N3, inputs);

  printf("output for {%d,%d} before training = %f\n", (int)inputs[0], (int)inputs[1], res);
  printf("Training...\n");

  PseudoTrain(N1, N2, N3, iterations, inputs, target);

  res = PseudoFeedForward(N1, N2, N3, inputs);

  printf("output for {%d,%d} after training = %f\n\n", (int)inputs[0], (int)inputs[1], res);
}

int main()
{
  srand(time(NULL));
  double inputs[][4] = {{0,0},{0,1},{1,0},{1,1}};
  double targets[4] = {0,1,1,0};
  double res;

  Neuron N1 = GenerateNeuron();
  Neuron N2 = GenerateNeuron();
  Neuron N3 = GenerateNeuron();
  Neuron A1 = N1;
  Neuron B1 = N1;
  Neuron C1 = N1;
  Neuron D1 = N1;
  Neuron A2 = N2;
  Neuron B2 = N2;
  Neuron C2 = N2;
  Neuron D2 = N2;
  Neuron A3 = N3;
  Neuron B3 = N3;
  Neuron C3 = N3;
  Neuron D3 = N3;

  DisplayProgress(&A1, &A2, &A3, 100000, inputs[0], targets[0]);
  DisplayProgress(&B1, &B2, &B3, 100000, inputs[1], targets[1]);
  DisplayProgress(&C1, &C2, &C3, 100000, inputs[2], targets[2]);
  DisplayProgress(&D1, &D2, &D3, 100000, inputs[3], targets[3]);

  return 0;
}
