/*----------------------------------------------
 * Author: Bridge Schaad 
 * Date: 2/17/2023
 * Description: A short test of read_ppm.c
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main() {
    int w;
    int h;
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

    // todo: print out the grid of pixels
    printf("Testing file feep-raw.ppm: %d %d\n", w, h);
    for (int i = 0; i < w; i++) { // rows
	for (int j = 0; j < h; j++) { // columns
	    printf("(%d,%d,%d) ", pixels[i * h + j].red, pixels[i*h + j].green, 
	    pixels[i*h +j].blue);
	}
	printf("\n");
    }
    free(pixels);
    return 0;
}

