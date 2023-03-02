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

/*
In the file, encode.c, write a program that reads in a PPM file (raw, or binary, format) and asks the user for a message to embed within it.

$ make encode
gcc -g -Wall -Wvla -Werror encode.c read_ppm.c -o encode
$ ./encode feep-raw.ppm
Reading feep-raw.ppm with width 4 and height 4
Max number of characters in the image: 5
Enter a phrase: lol
Writing file feep-raw-encoded.ppm
Requirements/Hints:

You should read the PPM filename as a command line argument

You should report the usage if no file is given to the program

You should report an error if the file cannot be read

You should output a new file based on the input name. For example, if the input is feep-raw.ppm, the new file with the encoded message should be feep-raw-encoded.ppm.

Re-use your implementation of read_ppm from Assignment 05.

Output the size of the image along with the maximum number of characters it can store

For debugging, try printing out values in hex format, e.g. printf("%02X", c);
*/

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  char* filename = argv[1];
  char output_filename[strlen(filename) + 12]; // 12 is the length of "-encoded.ppm"
  
  int w, h;
  struct ppm_pixel* img = read_ppm(filename, &w, &h);
  if (img == NULL) {
    printf("Error reading file %s (does it exist?)\n", filename);
    return 0;
  }

  int max_chars = w * h * 3 / 8; // 3 bytes per pixel, 8 bits per byte
  printf("Reading %s with width %d and height %d (max %d characters) \n", filename, w, h, max_chars);
  printf("Max number of characters in the image: %d", max_chars);

  char message[max_chars + 1]; // +1 for null terminator
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