#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <sys/time.h>


int size = 480;
float xmin = -2.0;
float xmax = 0.47;
float ymin = -1.12;
float ymax = 1.12;
int maxIterations = 1000;
int numProcesses = 4;
unsigned int **counts, max_count;
pthread_mutex_t count_mutex;
pthread_barrier_t barrier;
int numThreads = 4;

typedef struct {
    int startX, endX, startY, endY;
} ThreadData;
  
struct ppm_pixel *image;   

int mandelbrot(float x0, float y0) {
    float x = 0, y = 0;
    int iter = 0;
    while (iter < maxIterations && x * x + y * y < 4) {
        float xtmp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtmp;
        iter++;
    }
    return iter;
}

void *thread_start_routine(void *data) {
    ThreadData *td = (ThreadData *)data;
    int startX = td->startX, endX = td->endX, startY = td->startY, endY = td->endY;
    float xScale = (xmax - xmin) / size;
    float yScale = (ymax - ymin) / size;

     printf("Thread %p) sub-image block: cols (%d, %d) to rows (%d,%d)\n",
           (void*)pthread_self(), startX, endX, startY, endY);

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            float x0 = xmin + x * xScale;
            float y0 = ymax - y * yScale;
            int value = mandelbrot(x0, y0);

            if (value < maxIterations) { 
                float x = 0, y = 0;
                for (int iter = 0; iter < value; iter++) {
                    float xtmp = x * x - y * y + x0;
                    y = 2 * x * y + y0;
                    x = xtmp;

                    int xcol = round(size * (x - xmin) / (xmax - xmin));
                    int yrow = round(size * (y - ymin) / (ymax - ymin));

                    if (xcol >= 0 && xcol < size && yrow >= 0 && yrow < size) {
                        pthread_mutex_lock(&count_mutex);
                        counts[yrow][xcol]++;
                        if (counts[yrow][xcol] > max_count) {
                            max_count = counts[yrow][xcol];
                        }
                        pthread_mutex_unlock(&count_mutex);
                    }
                }
            }
        }
    }

    pthread_barrier_wait(&barrier);

    float gamma = 0.681;
    float factor = 1.0 / gamma;
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            float value = 0;

            if (counts[y][x] > 0) {
                value = log((double)counts[y][x]) / log(max_count);
                value = pow(value, factor);
            }

            struct ppm_pixel color = {
                .red = value * 255,
                .green = value * 255,
                .blue = value * 255,
            };

            image[y * size + x] = color;
        }
    }

  printf("Thread %p) finished\n", (void *)pthread_self());
  return NULL;
}

int main(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  counts = malloc(size * sizeof(unsigned int *));
  for (int i = 0; i < size; i++) {
    counts[i] = (unsigned int *)calloc(size, sizeof(unsigned int));
   }

  image = (struct ppm_pixel *)calloc(size * size, sizeof(struct ppm_pixel));

  pthread_mutex_init(&count_mutex, NULL);
    pthread_barrier_init(&barrier, NULL, numThreads);

    pthread_t *threads;
    ThreadData *thread_data;

    threads = (pthread_t *)malloc(numThreads * sizeof(pthread_t));
    thread_data = (ThreadData *)malloc(numThreads * sizeof(ThreadData));
    

    struct timeval start, end; // start and end timers
    gettimeofday(&start, NULL); // start timer

    int blockSizeX = size / 2;
    int blockSizeY = size / 2;

    for (int i = 0; i < numThreads; i++) {
        thread_data[i].startX = (i % 2) * blockSizeX;
        thread_data[i].endX = thread_data[i].startX + blockSizeX;
        thread_data[i].startY = (i / 2) * blockSizeY;
        thread_data[i].endY = thread_data[i].startY + blockSizeY;

        pthread_create(&threads[i], NULL, thread_start_routine, &thread_data[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }


    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, elapsed);
   

    char filename[128];
    snprintf(filename, sizeof(filename), "buddhabrot-%d-%ld.ppm", size, time(0));
    write_ppm(filename, image, size, size);
    printf("Writing file: %s\n", filename);
    
    for (int i = 0; i < size; i++) {
        free(counts[i]);
    }

    free(counts);
    free(image);
    free(threads);
    free(thread_data);

    pthread_mutex_destroy(&count_mutex);
    pthread_barrier_destroy(&barrier);
    return 0;
    
}

