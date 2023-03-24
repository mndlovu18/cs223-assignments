/*----------------------------------------------
 * Author: Minolta Ndlovu
 * Date: 03/24/2023
 * Description: This program outputs a PPM image 
 * of the mandelbrot set
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

/*
  * This function computes the mandelbrot set
  * @param real: the real part of the complex number
  * @param imag: the imaginary part of the complex number
  * @param maxIterations: the maximum number of iterations
  * @return: the number of iterations
*/
int mandelbrot(float real, float imag, int maxIterations) {
  int n;
  float r = 0.0, i = 0.0, rr = 0.0, ii = 0.0;
  for (n = 0; n < maxIterations && rr + ii <= 4.0; n++) {
    i = 2 * r * i + imag;
    r = rr - ii + real;
    rr = r * r;
    ii = i * i;
  }
  return n;
}

/*
  * This function writes the image to a PPM file
  * @param argc: the number of command line arguments
  * @param argv: the command line arguments
  * @return: 0 if successful
*/
int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }

  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  struct timeval start, end;
  gettimeofday(&start, NULL);

  struct ppm_pixel* image = (struct ppm_pixel*) malloc(size * size * sizeof(struct ppm_pixel));
  float xScale = (xmax - xmin) / size;
  float yScale = (ymax - ymin) / size;

  srand(time(0));

  for (int y = 0; y < size; y++) { //for each row
    for (int x = 0; x < size; x++) { //for each column
      float real = xmin + x * xScale;
      float imag = ymax - y * yScale;
      int value = mandelbrot(real, imag, maxIterations); //compute the mandelbrot set

      image[y * size + x] = (struct ppm_pixel){ //assign the pixel
        .red = (value * 5) % 256,
        .green = (value * 10) % 256,
        .blue = (value * 20) % 256
      };
    }
  }

  gettimeofday(&end, NULL); //end the timer
  double elapsed = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec); 
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, elapsed);

  char filename[128];
  snprintf(filename, sizeof(filename), "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm(filename, image, size, size);  //write the image to a PPM file
  printf("Writing file: %s\n", filename);

  free(image); 
  return 0; 
} 

