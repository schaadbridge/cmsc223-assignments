/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>

int main() {
    unsigned long img;
    scanf(" %lx", &img);
    printf("Image (unsigned long): %lx\n", img);

    // todo: your code here
    for (int i = 7; i >= 0; i--) { // rows
	for (int j = 0; j <= 1; j++) { // left/right
	    for (int k = 0; k < 4; k++) { // columns
		// **make mask
		unsigned long mask = 0x1ul << (i * 8 + j * 4 + k);
		// if mask & img == 1
		if ((mask & img) >> (i * 8 + j * 4 + k) == 1) {
		    printf("@ ");
		}
		// else, print " "
		else {
		    printf("  ");
		}
	    }
	}
	printf("\n");
    }

  return 0;
}
