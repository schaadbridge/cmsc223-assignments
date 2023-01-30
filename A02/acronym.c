/**
 *  Acronym.c
 *  Computes an acronym from a user-inputted phrase.
 *  Created by: Bridge Schaad
 *  Due: 2/1/2023
 */

#include <stdio.h>
#include <string.h>

int main() {
	printf("Enter a phrase: ");
	char phrase[1024];
	scanf("%[^\n]%*c", phrase);
	int phrase_len = strlen(phrase);
	char ret[1024]; //acronym cannot be longer than phrase
	int ret_len = 0;

	for (int i  = 0; i < phrase_len; i++) {
		if (phrase[i] > 64 && phrase[i] < 91) { // if uppercase
			ret[ret_len] = phrase[i];
			ret_len ++;
		}
	}

	ret[ret_len] = '\0';
	printf("Your acronym is %s\n", ret);
	return 0;
}
