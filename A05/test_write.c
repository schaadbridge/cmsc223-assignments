/*----------------------------------------------
 * Author: Bridge Schaad 
 * Date: 2/21/2023
 * Description: A short test of read_ppm.c.
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
    int w, h;
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

    // test writing the file to test.ppm
    char* filename = {"test.ppm"};
    write_ppm(filename, pixels, w, h);
    free(pixels);
    pixels = NULL;

    // reload pixels
    pixels = read_ppm("test.ppm", &w, &h);
    printf("Testing file feep-raw.ppm: %d %d\n", w, h);

    // print contents
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
