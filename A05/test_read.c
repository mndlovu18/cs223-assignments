/*----------------------------------------------
 * Author:Minolta Ndlovu
 * Date:02/22/2022
 * Description: This program reads a ppm file and returns the image data
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

/**
 * @brief Prints the pixels to the screen
 * 
 * @param pixels The pixels to print 
 * @param w  The width of the image
 * @param h  The height of the image
 */
void print_pixels(struct ppm_pixel* pixels, int w, int h) {
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      printf("(%d,%d,%d) ", pixels[i*w + j].red, 
        pixels[i*w + j].green, pixels[i*w + j].blue);
    }
    printf("\n");
  }
}

/**
 * @brief This is the main driver for the program
 * to print pixels of a ppm file
 * 
 * @return int 
 */
int main() {
  int w, h;
  struct ppm_pixel* pixels = 
    read_ppm("feep-raw.ppm", &w, &h); // read the file
  if (!pixels) {
        fprintf(stderr, "Failed to read PPM file\n");
        exit(1);
    }
  
  printf("Testing file feep-raw.ppm: %d %d\n", w, h); 
  print_pixels(pixels, w, h); // print the pixels

  free(pixels); // free the memory
  return 0;
}



