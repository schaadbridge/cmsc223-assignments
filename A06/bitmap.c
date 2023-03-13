/*----------------------------------------------
 * Author: Bridge Schaad 
 * Date: 3/1/23
 * Description: Read a 64-bit unsigned int and output an 8x8 1bpp sprite.:w`
 ---------------------------------------------*/
#include <stdio.h>

int main() {
    unsigned long img;
    scanf(" %lx", &img);
    printf("Image (unsigned long): %lx\n", img);

    for (int i = 7; i >= 0; i--) { // rows
        for (int j = 0; j <= 1; j++) { // left/right
            for (int k = 0; k < 4; k++) { // columns
                                          // **make mask
                unsigned long mask = 0x1ul << (i * 8 + j * 4 + k);
                // if 1, print @
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
