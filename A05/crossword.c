/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** printArr prints the contents of a 2d array */
void printArr(char* two_d_array, int len1, int len2) {
    for (int i = 0; i < len1; i++) { // rows
	for (int j = 0; j < len2; j++) { // columns
	    printf("%c", two_d_array[i*len2 + j]);
	}
	printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
	printf("Usage: %s <word1> <word2>\n", argv[0]);
	exit(0);
    }
    
    /** Determine presence of common chars. */
    int* freq1 = malloc(sizeof(int) * 26);
    for (int i = 0; i < 26; i++) {
	freq1[i] = 0;
    }
    int len1 = strlen(argv[1]);
    for (int i = 0; i < len1; i ++) { // store frequency of letters a-z in word1
	freq1[argv[1][i] - 'a']++;
    }

    int len2 = strlen(argv[2]);
    int pos2 = -1; // stores pos of commonChar in word2
    for (int i = 0; i < len2; i++) {
	if (freq1[argv[2][i] - 'a'] >=1) {
	    pos2 = i;
	    break;
	}
    }
    free(freq1);
    freq1 = NULL;

    if (pos2 < 0) { 
	printf("No common letter!\n");
    }
    else {
	char* two_d_array = malloc(len1 * len2);
	int pos1 = -1; // stores pos of commonChar in word1
	for (int i = 0; i < len1; i++) {
	    if (argv[1][i] == argv[2][pos2]) {
		pos1 = i;
		break;
	    }
	}
	// put this in a function eventually
	for (int i = 0; i < len1; i++) { // rows
	    for (int j = 0; j < len2; j++) { // columns
		if (i == pos1) {
		    two_d_array[i*len2 + j] = argv[2][j];
		}
		else if (j == pos2) {
		    two_d_array[i*len2 + j] = argv[1][i];
		}
		else {
		    two_d_array[i*len2 + j] = '.';
		}
	    }
	}
	// write a function to print contents of the array
	printArr(two_d_array, len1, len2);
	free(two_d_array);
    }

    return 0;
}
