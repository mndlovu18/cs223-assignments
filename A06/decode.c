/*----------------------------------------------
 * Author: Minolta Ndlovu
 * Date: 02/28/2023
 * Description: This program reads in a PPM file 
 * (raw, or binary, format) and then outputs any 
 * message that might be stored in the least 
 * significant bits of each color. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "read_ppm.h"
#include <string.h>

/**
* @brief This is the main driver for the program
* to read in a PPM file and output any message
* that might be stored in the least significant
* bits of each color.
* @param argc The number of arguments
* @param argv The arguments
* @return int 
*/
int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }
  int w, h;
  char *filename = argv[1];
  struct ppm_pixel* img = read_ppm(filename, &w, &h);

  if (img == NULL) {
    printf("Error reading file %s\n", filename);
    return 0;
  } 

  printf("Reading %s with width %d and height %d\n", filename, w, h);

  int max_chars = (w * h * 3) / 8; // 8 bits per char
  printf("Max number of characters in the image: %d\n", max_chars);

  char message[max_chars + 1]; // +1 for null terminator
  memset(message, 0, max_chars + 1); // initialize to all 0s

  int bit_index = 0;
  int char_index = 0;
  unsigned char c = 0;

  for (int i = 0; i < w * h; i++) { // loop over all pixels to get bits
    struct ppm_pixel* p = &img[i];

    c = c << 1; // shift left by 1
    c |= (p->red & 0x1); // set the least significant bit to the red bit
    bit_index++;

    if (bit_index == 8) { // we have a full character
      if (char_index < max_chars) { // make sure we don't overflow
        message[char_index] = c;
        char_index++;
      }
      if (c == '\0') { // we have reached the end of the message
        break;
      }
      bit_index = 0;
      c = 0;
    }

    c = c << 1; // shift left by 1
    c |= (p->green & 0x1); // set the least significant bit to the green bit
    bit_index++;

    if (bit_index == 8) { // we have a full character
      if (char_index < max_chars) { // make sure we don't overflow
        message[char_index] = c;
        char_index++;
      }
      if (c == '\0') { // we have reached the end of the message
        break;
      }
      bit_index = 0;
      c = 0;
    }

    c = c << 1; // shift left by 1
    c |= (p->blue & 0x1); // set the least significant bit to the blue bit
    bit_index++;

    if (bit_index == 8) { // we have a full character
      if (char_index < max_chars) { // make sure we don't overflow
        message[char_index] = c;
        char_index++;
      }
      if (c == '\0') { // we have reached the end of the message
        break;
      }
      bit_index = 0;
      c = 0;
    }
  }

  printf("%s\n", message);
  free(img);

  return 0;
}
