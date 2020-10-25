#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include "neuronX.c"


void printinfo(Neuron * N)
{
  printf("weight1 = %f, weight2 = %f\n", N -> weights[0], N -> weights[1]);
  printf("output for N = %f\n", N -> output);
}

double PseudoFeedForward(Neuron * N1, Neuron * N2, Neuron * N3, double inputs[])
{
  double output1[2];
  double output2;
  output1[0] = ProcessNeuron(inputs, N1);
  output1[1] = ProcessNeuron(inputs, N2);
  output2 = ProcessNeuron(output1, N3);
  return output2;
}

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

int main()
{
  srand(time(NULL));
  double inputs[][4] = {{0,0},{0,1},{1,0},{1,1}};
  double targets[4] = {0,1,1,0};
  double res;

  Neuron N1, A1, B1, C1, D1 = GenerateNeuron();
  Neuron N2, A2, B2, C2, D2 = GenerateNeuron();
  Neuron N3, A3, B3, C3, D3 = GenerateNeuron();

  printinfo(&N1);
  printinfo(&N2);

  res = PseudoFeedForward(&N1, &N2, &N3, inputs[1]);

  printinfo(&N1);
  printinfo(&N2);

  printf("output for {0,1} before training = %f\n", res);
  printf("Training...\n");

  PseudoTrain(&N1, &N2, &N3, 10000, inputs[1], targets[1]);

  res = PseudoFeedForward(&N1, &N2, &N3, inputs[1]);

  printf("output for {0,1} after training = %f\n", res);

  /*printf("output for {0,0} before training = %f\n", res);

  res = PseudoFeedForward(&N1, &N2, &N3, inputs[0]);
  printf("Training...\n");

  resfor1 = PseudoTrain(N1, N2, N3, 10000, inputs[0], targets[0]);

  res = PseudoFeedForward(&N1, &N2, &N3, inputs[0]);

  printf("output for {0,0} after training = %f\n", res);

  printf("Testing\n");

  res = PseudoFeedForward(&N1, &N2, &N3, inputs[1]);

  printf("output for {0,1} after training = %f\n", res);*/

  return 0;
}
