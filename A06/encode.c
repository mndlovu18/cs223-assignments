/*----------------------------------------------
 * Author: Minolta Ndlovu
 * Date: 2/28/2023
 * Description: This program reads in a PPM file 
 * (raw, or binary, format) and asks the user for 
 * a message to embed within it.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

/**
 * @brief This is the main driver function for the program
 * that reads in a PPM file (raw, or binary, format) and
 * asks the user for a message to embed within it.
 * @param argc The number of arguments
 * @param argv The arguments
 * @return int 
 */
int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  char* filename = argv[1];
  char output_filename[256]; 
  
  int w, h;
  struct ppm_pixel* img = read_ppm(filename, &w, &h);
  if (img == NULL) {
    printf("Error reading file %s (does it exist?)\n", filename);
    return 0;
  }

  int max_chars = w * h * 3 / 8; // 3 bytes per pixel, 8 bits per byte
  printf("Reading %s with width %d and height %d (max %d characters) \n", filename, w, h, max_chars);
  printf("Max number of characters in the image: %d\n", max_chars);

  char message[1024]; 
  memset(message, 0, max_chars + 1); // initialize to all 0s

  printf("Enter a phrase: ");
  fgets(message, max_chars + 1, stdin);
  message[strlen(message) - 1] = '\0'; // remove the newline

  int message_len = strlen(message);
  if (message_len > max_chars) {
    printf("Message is too long for the image (max %d characters)\n", max_chars);
    return 0;
  }

  int bit_index = 0;
  for (int i = 0; i < w * h; i++) {
    struct ppm_pixel* p = &img[i];
    if (bit_index < message_len * 8) {
      int bit_value = (message[bit_index / 8] >> (bit_index % 8)) & 1;
      if (bit_value != 0) {
        p->red |= 1; 
      } else {
        p->red &= ~1;
      }
      bit_index++;
    } else {
      break;
    }
  }

  sprintf(output_filename, "%s-encoded.ppm", filename);
  write_ppm(output_filename, img, w, h);
  printf("Writing file %s \n", output_filename);

  free(img);
  return 0;
}