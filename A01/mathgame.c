/***************************************************
 * mathgame.c
 * Author: Bridge Schaad
 * Implements a math game
 * Created: 1/23/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int num_rounds, corr_rounds;/**num_rounds: number of rounds
	     			 corr_rounds: number of correct rounds */
  printf("Welcome to Math Game!\n");
  printf("How many rounds do you want to play? ");
  scanf("%d", &num_rounds);


/**Figure out how to generate two random digits 1-10*/
  for (int i = 0; i < num_rounds; i++) {
	  int given_ans;
	  time_t t;
	  srand((unsigned) time(&t)); /** initialize a random number generator */
	  int random1 = rand() % 10;  /**generate random number 1-10 */
	  int random2 = rand() % 10;
 	  printf("%d + %d = ? ", random1, random2);
	  scanf("%d", &given_ans);

	  if (given_ans == random1+random2) {
		corr_rounds++;
		printf("Correct!\n");
	  }
	  else {
		printf("Incorrect :(\n");
	  }
  }
  printf("You answered %d/%d correctly.\n", curr_rounds, num_rounds);
  return 0;
}
