/*----------------------------------------------
 * Author:Minolta Ndlovu
 * Date:02/22/2023
 * Description:This program writes a ppm file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

/**
 * @brief   This function writes a ppm file
 * 
 * @param filename  The name of the file to write
 * @param pixels  The pixels to write
 * @param w  The width of the image
 * @param h  The height of the image
 */
void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Failed to open file %s for writing\n", filename);
        return;
    }
    fprintf(fp, "P6\n%d %d\n255\n", w, h);
    fwrite(pixels, sizeof(struct ppm_pixel), w*h, fp);
    fclose(fp);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
