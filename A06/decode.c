/*----------------------------------------------
 * Author: Bridge Schaad 
 * Date: 3/1/2023
 * Description: Decode last bit of r,g,b from each pixel of ppm file.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
    if (argc != 2) {
	printf("usage: decode <file.ppm>\n");
	return 0;
    }

    // read in ppm file
    int w;
    int h;
    struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
    
    if (pixels == NULL) {
	printf("File could not be read. Exiting.\n");
	exit(1);
    }
    
    printf("Reading %s with width %d and height %d\n", argv[1], w, h);
    unsigned char* bitstring = malloc(w * h * 3 / 8 + 1); // total rgb vals * 1 char / 8 bits
    for (int i = 0; i < (w * h * 3 / 8); i++) {
	bitstring[i] = 0;
    }
    int max_chars = w * h * 3 / 8;
    printf("Max number of characters in the image: %d\n", max_chars);

    // fill bitstring
    for (int i = 0; i < w * h * 3; i++) {
	bitstring[i / 8] = bitstring[i / 8] << 1; // make space in least digit
	char code;
	if (i % 3 == 0) {
	    code = pixels[i / 3].red & 0x01;
	}
	else if (i % 3 == 1) {
	    code = pixels[i / 3].green & 0x01;
	}
	else if (i % 3 == 2) {
	    code = pixels[i / 3].blue & 0x01;
	}
	bitstring[i / 8] = bitstring[i / 8] | code;
    }
    
    printf("%s\n", bitstring);    
    
    free(pixels);
    free(bitstring);
    return 0;
}

