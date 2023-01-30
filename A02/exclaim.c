/** exclaim.c
 *  For Assignment 2: turn a word into an exclaimation, replacing lowercase
 *  letter with other char
 *  Created by: Bridge Schaad
 *  Due: 2/1/2023
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
	char buff[32]; // assume input fits 32 char buffer
	printf("Enter a word: ");
	scanf("%s", buff);
	int buff_len = strlen(buff);
	char censor[4] = {'@', '!', '#', '*'};

	// while next char != '\0'
	srand(time(NULL));
	for (int i = 0; i < buff_len; i++) {
		if (buff[i] > 96 && buff[i] < 123) { // if next letter is lowercase
			buff[i] = censor[(rand() % 4)]; // replace lowercase with 1 of 4 censor chars
		}
	}
	printf("OMG! %s!\n", buff);

	return 0; 
}
