/*----------------------------------------------
 * Author: Bridge Schaad 
 * Date: 2/28/23
 * Description: Write an array of pixels into a ppm file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

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
