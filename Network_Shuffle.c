#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include "Layer.c"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixeloperations.c"
#include "err.h"


////////////////////// SDL //////////////////////////////

void init_SDL()
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s. \n", SDL_GetError());

}



//Load the image in the memory
SDL_Surface* load_image(char* path)
{
    SDL_Surface *img;

    img = IMG_Load(path);
    if(!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
    return img;
}



void pixel_values(SDL_Surface* img, double pixels[])
{
  Uint8 pixel;
  Uint8 r, g, b;
  int height = img->h;
  int width = img->w;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      pixel = get_pixel(img, j, i);
      pixels[i * 28 + j] = pixel/255;
    }

  }

}

//////////////////////////////////////////////////////////

struct Network
{
  Layer layers[3];
  double *output;
};

///////Declarations
typedef struct Network Network;
double* FeedForwardXOR(double* input, Network net);
double* FeedForward(double* input, Network net);
/////////

/////Generates 3 layered Network ///////////
Network GenerateNetwork()
{
  double nb_neurons[3] = {70, 50, 26};
  double nb_inputs[3] = {784, 70, 50};
  Network net;
  for (size_t i = 0; i < 3; i++)
  {
    net.layers[i] = GenerateLayer(nb_neurons[i], nb_inputs[i]);
  }

  return net;
}

//////// Feed Forward ///// Called after every epoch //////
double* FeedForward(double* input, Network net)
{
  double nb_inputs = 784;
  net.output = input;
  for(size_t i = 0; i < 3; i++)
  {
    net.output = ProcessLayer(net.output, &net.layers[i]);
    nb_inputs = net.layers[i].nb_outputs;
  }
  return net.output;
}

//// BACK PROPAGATION //////////////
void BackPropagation(Network net, double *target, double* inputs)
{
  double error;
  double dl_dw;
  double dl_db;
  Neuron* neuron;

  int k = 2;
  while (k >= 0)
  {
    for (int i = 0; i < net.layers[k].nb_outputs; i++)
    {
      neuron = &net.layers[k].neurons[i];
      for (int j = 0; j < neuron->nb_weights; j++)
      {
        // If iterating over last layer
        if (k == 2)
        {
          error = -(target[i] - neuron->output);
          dl_dw = error * sigmoid_derivative(neuron->output) * net.layers[k - 1].neurons[j].output;
          dl_db = error * sigmoid_derivative(neuron->output) * 1;
          net.layers[k - 1].neurons[j].dl_wrt_curr += neuron->weights[j] * sigmoid_derivative(neuron->output) * error;

          neuron->weights[j] = neuron->weights[j] - 0.3 * dl_dw;
          neuron->bias = neuron->bias - 0.3 * dl_db;
        }
        else if (k > 0)
        {
          dl_dw = neuron->dl_wrt_curr * sigmoid_derivative(neuron->output) * net.layers[k - 1].neurons[j].output;
          dl_db = neuron->dl_wrt_curr * sigmoid_derivative(neuron->output) * 1;
          net.layers[k - 1].neurons[j].dl_wrt_curr += neuron->weights[j] * sigmoid_derivative(neuron->output) * neuron->dl_wrt_curr;

          neuron->weights[j] = neuron->weights[j] - 0.3 * dl_dw;
          neuron->bias = neuron->bias - 0.3 * dl_db;
        }
        else
        {
          dl_dw = neuron->dl_wrt_curr * sigmoid_derivative(neuron->output) * inputs[j];
          dl_db = neuron->dl_wrt_curr * sigmoid_derivative(neuron->output) * 1;
          neuron->weights[j] = neuron->weights[j] - 0.3 * dl_dw;
        }
      }

    }
    k--;

  }

}


/// Train The mode. Preparation for prediction
void Train_model(double* inputs, double *target, Network net, size_t iterations)
{
  double *res;
  for (int i = 0; i < iterations; i++)
  {
    res = FeedForward(inputs, net);
    printf("%i epoch\n", i);
    BackPropagation(net, target, inputs);

  }

}

/////////// Prediction Function /////////////
void Prediction(double* result, char letters[])
{
    for (size_t i = 0; i < 26; i++)
    {
      if (result[i] > 0.5)
      {
        printf("Predicted by the model: %c\n", letters[i]);
        break;
      }
    }
}

////////////Saving weights and biases in a txt file///////////

void write_file(Layer layer, char* filename)
{
  FILE *file;
  file = fopen(filename, "a");
  for (size_t i = 0; i < layer.nb_outputs; i++)
  {

    for (size_t j = 0; j < layer.neurons -> nb_weights; j++)
    {
      fprintf (file, "%f\n", (layer.neurons[i].weights)[j]);
    }
    fprintf(file, "%f\n", layer.neurons[i].bias);
  }
  fclose(file);
}

////////////reading weights and biases in a txt file///////////
void read_file(Layer l, int nb_bias, int nb_weigths, char* filename)
{
  FILE *file;
  file = fopen(filename, "r");
  int r = nb_bias + nb_weigths;
  double data[r];
  int i = 0;
  while (!feof(file))
  {
    fscanf(file, "%lf", &data[i]);
    i += 1;
  }
  fclose(file);
  i = 0;
  for (size_t j = 0; j < l.nb_outputs; j++)
  {
    //printf(" neuron %d\n", i);
    for (size_t k = 0; k < l.neurons -> nb_weights; k++)
    {
      (l.neurons[j].weights)[k] = data[i];
      i += 1;
    }
    l.neurons[j].bias = data[i];
    i += 1;
  }

}

//////////////////shuffling function////////////////////////

void shuffle(int *array,int *targets[], size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

int main()
{

  SDL_Surface *img;
  srand(time(NULL));

  //double inputXOR[2] = {0, 0};
  //double *inputs_xor = inputXOR;

  img = load_image("1char.bmp");

  int inputsindices[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

  double target1[26] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t1 = target1;
  double target2[26] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t2 = target2;
  double target3[26] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t3 = target3;
  double target4[26] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t4 = target4;
  double target5[26] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t5 = target5;
  double target6[26] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t6 = target6;
  double target7[26] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t7 = target7;
  double target8[26] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t8 = target8;
  double target9[26] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t9 = target9;
  double target10[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t10 = target10;
  double target11[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t11 = target11;
  double target12[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t12 = target12;
  double target13[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t13 = target13;
  double target14[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t14 = target14;
  double target15[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t15 = target15;
  double target16[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t16 = target16;
  double target17[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t17 = target17;
  double target18[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  double *t18 = target18;
  double target19[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
  double *t19 = target19;
  double target20[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
  double *t20 = target20;
  double target21[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
  double *t21 = target21;
  double target22[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
  double *t22 = target22;
  double target23[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
  double *t23 = target23;
  double target24[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
  double *t24 = target24;
  double target25[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
  double *t25 = target25;
  double target26[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  double *t26 = target26;

  double *targets[26] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26};

  //double target[26] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


  double pixels[784];
  double* result;

  double *result2;

  pixel_values(img, pixels);
  double *inputs_ocr = pixels;

  char letters[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

  /// INIT NETWORK //
  Network OCR = GenerateNetwork();
  Network OCR2 = GenerateNetwork();

  // First FEED Forward ////////
  result = FeedForward(inputs_ocr, OCR);

  for (size_t i = 0; i < 26; i++)
  {
    printf("%f\n", *result);
    result++;
  }


  printf(">>>>>>>................<<<<<<<<<<\n");

  ///////// Train the model as per number of iterations /////////////////
  Train_model(inputs_ocr, targets[23], OCR, 500);

  printf(">>>>>>>>>>>Model Trained<<<<<<<<<<<<\n");


  /// Final FEED Forward ////////////
  result = FeedForward(inputs_ocr, OCR);

  for (size_t i = 0; i < 26; i++)
  {
    printf("%f\n", result[i]);
  }


  // Do the Prediction /////////////
  Prediction(result, letters);

  write_file(OCR.layers[0], "layer1data");
  write_file(OCR.layers[1], "layer2data");
  write_file(OCR.layers[2], "layer3data");

  read_file(OCR2.layers[0], 70, 70*784, "layer1data");
  read_file(OCR2.layers[1], 50, 50*70, "layer2data");
  read_file(OCR2.layers[2], 26, 26*50, "layer3data");

  result2 = FeedForward(inputs_ocr, OCR2);
  for (size_t i = 0; i < 26; i++)
  {
    printf("%f\n", result2[i]);
  }


  // Do the Prediction /////////////
  Prediction(result2, letters);

}
