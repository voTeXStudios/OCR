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
void BackPropagation(Network net, double target[], double* inputs)
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
void Train_model(double* inputs, double target[], Network net, size_t iterations)
{
  double *res;
  for (int i = 0; i < iterations; i++)
  {
    res = FeedForward(inputs, net);
    printf("%i epoch\n", i);
    BackPropagation(net, target, inputs);

  }

}


////////////////////////////////////////////// XOR SYSTEM STARTS HERE ////////////////////////////////////////////////////////////
void BackPropagationXOR(Network net, double target, double* initial_inputs)
{
  double error;
  double dl_dw;
  Neuron* neuron;
  int k = 1;
  while (k >= 0)
  {
    for (int i = 0; i < net.layers[k].nb_outputs; i++)
    {
      neuron = &net.layers[k].neurons[i];
      for (int j = 0; j < neuron->nb_weights; j++)
      {
        if (k == 1)
        {
          error = -(target - neuron -> output);
          dl_dw = error * sigmoid_derivative(neuron->output) * net.layers[k - 1].neurons[j].output;
          net.layers[k - 1].neurons[j].dl_wrt_curr += neuron->weights[j] * sigmoid_derivative(neuron->output) * error;

          neuron->weights[j] = neuron->weights[j] - 0.6 * dl_dw;
        }
        else
        {
          dl_dw = net.layers[k].neurons[j].dl_wrt_curr * sigmoid_derivative(neuron->output) * initial_inputs[j];

          neuron->weights[j] = neuron->weights[j] - 0.6 * dl_dw;
        }
      }
    }
    k--;
  }

}


void Train_model_XOR(double inp[], double* inputs, double target, Network net, double iterations)
{
  double *res;

  for (size_t i = 0; i < iterations; i++)
  {
    //printf("epochs\n");
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


double* FeedForwardXOR(double* input, Network net)
{
  double nb_inputs = 2;
  net.output = input;
  for(size_t i = 0; i < 2; i++)
  {
    net.output = ProcessLayer(net.output, &net.layers[i]);
    nb_inputs = net.layers[i].nb_outputs;
  }
  return net.output;
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

void write_file(Layer layer)
{
  FILE *file;
  file = fopen("Network data.txt", "a");
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
void read_file(Layer l)
{
  FILE *file;
  file = fopen("Network data.txt", "r");
  double data[30];
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
    printf(" neuron %ld\n", i);
    for (size_t k = 0; k < l.neurons -> nb_weights; k++)
    {
      (l.neurons[j].weights)[k] = data[i];
      i += 1;
    }
    l.neurons[j].bias = data[i];
    i += 1;
  }

}
//////////////////////////////////////////


int main()
{

  SDL_Surface *img;
  srand(time(NULL));

  //double inputXOR[2] = {0, 0};
  //double *inputs_xor = inputXOR;

  img = load_image("9char.bmp");
  double target[26] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double pixels[784];
  double* result;
  pixel_values(img, pixels);
  double *inputs_ocr = pixels;

  char letters[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

  /// INIT NETWORK //
  Network OCR = GenerateNetwork();

  // First FEED Forward ////////
  result = FeedForward(inputs_ocr, OCR);

  for (size_t i = 0; i < 26; i++)
  {
    printf("%f\n", *result);
    result++;
  }


  printf(">>>>>>>................<<<<<<<<<<\n");

  ///////// Train the model as per number of iterations /////////////////
  Train_model(inputs_ocr, target, OCR, 500);

  printf(">>>>>>>>>>>Model Trained<<<<<<<<<<<<\n");


  /// Final FEED Forward ////////////
  result = FeedForward(inputs_ocr, OCR);

  for (size_t i = 0; i < 26; i++)
  {
    printf("%f\n", result[i]);
  }


  // Do the Prediction /////////////
  Prediction(result, letters);


  Neuron n = GenerateNeuron(2);
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
  }



  for (size_t k = 0; k < 2 ; k++)
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

  double* result;
  result = FeedForwardXOR(inputs, XOR);
  printf("Before Training, output is = %f\n", *result);
  printf("..............\n");

  Train_modelXOR(arr, inputs, 0, XOR, 10000);

  result = FeedForwardXOR(inputs, XOR);

  printf("After Training, output is = %f\n", *result);
  //Prediction(result);



  for (size_t i = 0; i < 28; i++)
  {
    for (size_t j = 0; j < 28; j++)
    {
      printf("%f\n", pixels[i*28+j]);
    }


  }

  /*
  Layer l = GenerateLayer(5, 5);
  for (size_t i = 0; i < l.nb_outputs; i++)
  {
    printf(" neuron %ld\n", i);
    for (size_t j = 0; j < l.neurons -> nb_weights; j++)
    {
      printf(" weight%li = %f\n",j , (l.neurons[i].weights)[j]);
    }
    printf(" bias = %f\n", l.neurons[i].bias);
  }
  write_file(l);
  read_file(l);

  printf("------NEW LAYER-------");
  for (size_t i = 0; i < l.nb_outputs; i++)
  {
    printf(" neuron %ld\n", i);
    for (size_t j = 0; j < l.neurons -> nb_weights; j++)
    {
      printf(" weight%li = %f\n",j , (l.neurons[i].weights)[j]);
    }
    printf(" bias = %f\n", l.neurons[i].bias);
  }
  */


}
