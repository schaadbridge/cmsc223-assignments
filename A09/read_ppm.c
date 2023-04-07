#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
    FILE* infile = fopen(filename, "rb");
    if (infile == NULL) {
        return NULL;
    }

    // Read header
    char h1[4]; //first line of header
    fscanf(infile, "%s%*c", h1);
    char comment[256];
    fscanf(infile, "%[^\n]%*c", comment);

    fscanf(infile, "%d", w);
    fscanf(infile, "%d", h);
    int pixels = *w * *h;

    int maxval; // max color val
    fscanf(infile, "%d%*c", &maxval); // takes care of single newline

    // fill flat array
    struct ppm_pixel* new = malloc(sizeof(struct ppm_pixel) * pixels);
    if (new == NULL) {
        return NULL;
    }
    fread(new, sizeof(struct ppm_pixel), pixels, infile);

    fclose(infile);
    return new; 
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {

  return NULL;
}
