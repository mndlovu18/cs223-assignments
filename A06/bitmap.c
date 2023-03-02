/*----------------------------------------------
 * Author: Minolta Ndlovu
 * Date: 02/27/2023
 * Description: This program that reads in a 
 * single 64-bit unsigned integer (e.g. unsigned 
 * long) and outputs it as an 8x8 1bpp sprite.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief This is the main driver for the program
 * to read in a single 64-bit unsigned integer and
 * output an 8x8 1bpp sprite
 * @param argc The number of arguments
 * @param argv The arguments
 * @return int 
 */
int main(int argc, char *argv[]) {
    unsigned long img; // initialize the image variable
    scanf("%lx", &img); // read in the image from the file

    int row, column; // initialize the row and column variables
    for (row = 0; row < 8; row++) {
        for (column = 0; column < 8; column++) {
          int bit = (img >> (63 - (row * 8 + column))) & 0x1; // get 
            //the bit at the current position
            if (bit == 1) {
                printf("@");
            } else {
                printf(" ");
            }

        }
        printf("\n");
    }
    return 0;
}
