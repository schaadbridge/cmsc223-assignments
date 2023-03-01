/*----------------------------------------------
 * Author: Bridge Schaad 
 * Date: 3/1/2023
 * Description: Read in a PPM file and embed a user-provided message within it.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
    if (argc != 2) {
	printf("usage: encode <file.ppm>\n");
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
    printf("Reading %s with width %d and height %d.\n", argv[1], w, h);
    
    // get phrase
    int max_chars = (w * h * 3 / 8) - 1; // not including terminating char
 
    printf("Max number of characters in the image: %d\n", max_chars);
    char* phrase = malloc(max_chars);
    printf("Enter a phrase: "); 
    scanf("%[^\n]%*c", phrase);
    int phrase_len = strlen(phrase);

    if (phrase_len > max_chars) {
	printf("Phrase length is greater than max length. Exiting.\n");
	free(pixels);
	exit(1);
    }
    
    //encode phrase
    int idx;
    for (int i = 0; i < phrase_len; i++) { // for each letter
	for (int j = 0; j < 8; j++) { // for each bit
	    unsigned char least_mask = 0x01 << (7-j);
	    // bit mask to get jth dig as last val
	    char least_bit = (phrase[i] & least_mask) >> (7-j);
	    idx = i * 8 + j;
	    if (idx % 3 == 0) {
		pixels[idx / 3].red = pixels[idx / 3].red &  0xfe; // ensure last bit 1
		pixels[idx / 3].red = pixels[idx / 3].red | least_bit;
	    }
	    else if (idx % 3 == 1) {
		pixels[idx / 3].green = pixels[idx / 3].green & 0xfe;
		pixels[idx / 3].green = pixels[idx / 3].green | least_bit;
	    }
	    else if (idx % 3 == 2) {
		pixels[idx / 3].blue = pixels[idx / 3].blue & 0xfe;
		pixels[idx / 3].blue = pixels[idx / 3].blue | least_bit;
	    }
	}
    } 	

    // add terminating char
    for (int j = 0; j < 8; j++) { // for each bit
	idx ++;
	if (idx % 3 == 0) {
	    pixels[idx / 3].red = pixels[idx / 3].red & 0xfe;
	}
	else if (idx % 3 == 1) {
	    pixels[idx / 3].green = pixels[idx / 3].green & 0xfe;
	    }
	else if (idx % 3 == 2) {
	    pixels[idx / 3].blue = pixels[idx / 3].blue & 0xfe;
	}
    }
    char* mini_name = strtok(argv[1], ".");
    char* new_file = strcat(mini_name, "-encoded.ppm");

    
    printf("Writing file: %s\n", new_file);
    write_ppm(new_file, pixels, w, h);
    free(pixels);
    free(phrase);
    return 0;
}

