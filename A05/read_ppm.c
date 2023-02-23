/*----------------------------------------------
 * Author:Minolta Ndlovu 
 * Date:02/22/2023 
 * Description: This program reads a ppm file and returns the image data
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

/**
 * @brief   Reads a ppm file and returns the image data
 *
 * @param filename The name of the file to read
 * @param w  The width of the image
 * @param h  The height of the image
 * @return struct ppm_pixel*
 */
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
    FILE *fp = fopen(filename, "rb");  //read the file 
    if (fp == NULL) {
        printf("Error opening file %s \n", filename);
        return NULL;
    }
    char header[1024]; //P6
    fgets(header, 1024, fp); //read the header first line
    char comment[1024];
    fgets(comment, 10024, fp); //read the comment line

    int max; // max

    fgets(comment, 100, fp);
    sscanf(comment, "%d %d", w, h); //read the width and height

    if (fscanf(fp, "%d", &max) != 1) { //read the max value
        printf("Error reading max value \n");
        return NULL;
    }
    getc(fp);  //skip the whitespace

    struct ppm_pixel *image = malloc(*w * *h * 
        sizeof(struct ppm_pixel)); //allocate memory for the image
    if (image == NULL) {
        printf("Error allocating memory for image \n");
        return NULL;
    }

    //read the image data
    if (fread(image, sizeof(struct ppm_pixel), *w * *h, fp) != *w * *h) {
        printf("Error reading image data \n");
        free(image);
        return NULL;
    }

    fclose(fp); //close the file

    return image; //return the image

}


struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
