/*----------------------------------------------
 * Author:Minolta Ndlovu
 * Date:02/22/2022
 * Description: This program reads a ppm file
 * and returns the image data
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

/**
 * @brief  This function prints the pixels to the screen
 *
 * @param pixels  The pixels to print
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
 * @param argc  The number of arguments
 * @param argv  The arguments
 * 
 * @return int 
 */
int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  if (!pixels) {
        fprintf(stderr, "Failed to read PPM file\n");
        exit(1);
    }
  write_ppm("test.ppm", pixels, w, h);

  struct ppm_pixel* pixels2 = read_ppm("test.ppm", 
    &w, &h); // read in the file back

  printf("Testing file feep-raw.ppm: %d %d\n", w, h); 
  print_pixels(pixels2, w, h); // print the pixels
  free(pixels);
  free(pixels2);
  return 0;
}
