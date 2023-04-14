/*----------------------------------------------
 * Author:Minolta Ndlovu 
 * Date: 04/05/2023
 * Description: A program that outputs a PPM image 
 * of the mandelbrot set. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <sys/time.h>


/*
* A function that computes the number of iterations.
* @param real The real part of the complex number.
* @param imag The imaginary part of the complex number.
* @param max_iterations The maximum number of iterations.
* @return The number of iterations.
*/
int mandelbrot(float real, float imag, int max_iterations) {
  int n;
  float r = 0.0, i = 0.0, rr = 0.0, ii = 0.0;
  for (n = 0; n < max_iterations && rr + ii <= 4.0; n++) {
    i = 2 * r * i + imag;
    r = rr - ii + real;
    rr = r * r;
    ii = i * i;
  }
  return n;
}

/*
* A struct that holds the data for each thread.
*/
typedef struct {
    int id;
    int size;
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    int max_iterations;
    unsigned char* image;
} thread_data_t;

/*
* A function that computes the mandelbrot set for a given sub-image block.
* @param arg A pointer to the thread data.
* @return NULL
*/
void* compute_mandelbrot(void* arg) {
    thread_data_t *data = (thread_data_t*)arg;

    int start_col = (data->size / 2) * (data->id % 2);
    int end_col = start_col + data->size / 2;
    int start_row = (data->size / 2) * (data->id / 2);
    int end_row = start_row + data->size / 2;

    printf("Thread %p) sub-image block: cols (%d, %d) to rows (%d,%d)\n",
           (void*)pthread_self(), start_col, end_col, start_row, end_row);

     for (int row = start_row; row < end_row; ++row) { // for each row, compute the mandelbrot set
        for (int col = start_col; col < end_col; ++col) { //for each column, compute the mandelbrot set
            float x = data->xmin + (data->xmax - data->xmin) * col / (data->size - 1);
            float y = data->ymin + (data->ymax - data->ymin) * row / (data->size - 1);
            int value = mandelbrot(x, y, data->max_iterations);
          
            data->image[row * data->size * 3 + col * 3] = (value * 5) % 256; 
            data->image[row * data->size * 3 + col * 3 + 1] = (value * 10) % 256;
            data->image[row * data->size * 3 + col * 3 + 2] = (value * 20) % 256;
        }
    }

    printf("Thread %p) finished\n", (void *)pthread_self());
    return NULL;
}

/*
* This function writes the image to a PPM file.
* @param argc The number of command line arguments.
* @param argv The command line arguments.
* @return 0 on success, 1 on failure.
*/
int main(int argc, char* argv[]) {
    int size = 480;
    float xmin = -2.0;
    float xmax = 0.47;
    float ymin = -1.12;
    float ymax = 1.12;
    int max_iterations = 1000;
    int num_threads = 4;

    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
                             "-b <ymin> - t <ymax>\n", argv[0]); break;
        }
    }
    printf("Generating mandelbrot with size %dx%d\n", size, size);
    printf("  Num threads = %d\n", num_threads);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    unsigned char* image = malloc(3 * size * size * sizeof(unsigned char)); // allocate memory for the image

    thread_data_t* thread_data = malloc(num_threads * sizeof(thread_data_t)); // allocate memory for the thread data
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t)); // allocate memory for the threads
 
    struct timeval start, end; // start and end timers
    gettimeofday(&start, NULL); // start timer

    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = (thread_data_t){
            .id = i,
            .size = size,
            .xmin = xmin,
            .xmax = xmax,
            .ymin = ymin,
            .ymax = ymax,
            .max_iterations = max_iterations,
            .image = image
        };
        pthread_create(&threads[i], NULL, compute_mandelbrot, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, elapsed);

    char filename[256];
    sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
    write_ppm(filename, (struct ppm_pixel*)image, size, size);
    printf("Writing file: %s\n", filename);

    free(image);
    free(thread_data);
    free(threads);
    return 0;
}

