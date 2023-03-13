/*----------------------------------------------
 * Author: Bridge Schaad 
 * Date: 2/21/2023
 * Description: Write a pixel array into a new binary ppm file.
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// Write a new ppm using a flat implementation of a 2D array.
void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE* outfile = fopen(filename, "wb");

    // add header
    char h1[4] = {"P6"}; // first line
    fprintf(outfile, "%s\n", h1);
    char comment[50] = {"# Created by GIMP version 2.10.24 PNM plug-in"};
    fprintf(outfile, "%s\n", comment);
    fprintf(outfile, "%d %d\n255\n", w, h);

    // add pixels
    fwrite(pixels, sizeof(struct ppm_pixel), w*h, outfile);
    fclose(outfile);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
