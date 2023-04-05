#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
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
* The function computes the image
* @param image: the image
* @param size: the size of the image
* @param xmin: the minimum x value
* @param xmax: the maximum x value
*/
void compute_image(struct ppm_pixel* image, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations, int startX, int endX, int startY, int endY) {
    float xScale = (xmax - xmin) / size;
    float yScale = (ymax - ymin) / size;

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            float real = xmin + x * xScale;
            float imag = ymax - y * yScale;
            int value = mandelbrot(real, imag, maxIterations);

            image[y * size + x] = (struct ppm_pixel){
                .red = (value * 5) % 256,
                .green = (value * 10) % 256,
                .blue = (value * 20) % 256
            };
        }
    }
}

/* 
* This function writes the image to a PPM file
* @param argc: the number of command line arguments
* @param argv: the command line arguments
* @return: 0 if successful, 1 otherwise
*/

int main(int argc, char* argv[]) {
    int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

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

    int shm_id = shmget(IPC_PRIVATE, size * size * sizeof(struct ppm_pixel), IPC_CREAT | 0666);
    assert(shm_id != -1);
    struct ppm_pixel* image = (struct ppm_pixel*)shmat(shm_id, NULL, 0);
    assert(image != (void*)-1);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int blockSizeX = size / 2;
    int blockSizeY = size / 2;

    for (int i = 0; i < numProcesses; i++) {
        int startX = (i % 2) * blockSizeX;
        int endX = startX + blockSizeX;
        int startY = (i / 2) * blockSizeY;
        int endY = startY + blockSizeY;

        pid_t pid = fork();
        if (pid < 0) {
            perror("Error in fork");
            exit(1);
        }
        if (pid == 0) { // Child process
            printf("Launched child process: %d\n", getpid());
            printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), startX, endX, startY, endY);

            compute_image(image, size, xmin, xmax, ymin, ymax, maxIterations, startX, endX, startY, endY);

            exit(0);
        }
    }

    for (int i = 0; i < numProcesses; i++) { // Wait for child processes to complete
        int status;
        pid_t pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Child process complete: %d\n", pid);
        }
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, elapsed);

    char filename[128];
    snprintf(filename, sizeof(filename), "multi-mandelbrot-%d-%ld.ppm", size, time(0));
    write_ppm(filename, image, size, size);
    printf("Writing file: %s\n", filename);

    shmdt(image);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}