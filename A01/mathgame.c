/***************************************************
 * mathgame.c
 * Author: Bridge Schaad 
 * Implements a math game
 * Created: 1/22/2023
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
  int num_rounds, curr_rounds;/**num_rounds: number of rounds
	      corr_rounds: number of correct rounds */
  printf("Welcome to Math Game!\n");
  printf("How many rounds do you want to play? ");
  scanf("%d", &num_rounds);/**not sure if this will work, double check with notes!*/


/**Figure out how to generate two random digits 1-10*/
  for (int i = 0; i < num_rounds; i++) {
	  int given_ans;
	  int random1 = rand() % 10;
	  int random2 = rand() % 10;
	  printf("%d + %d = ? ", random1, random2);
	  scanf("%d", &given_ans); /**again, not sure. check.*/

	  if (given_ans == random1+random2) {
		curr_rounds++;
		printf("Correct!\n");
	  }
	  else {
		printf("Incorrect :(\n");
	  }
  }
  printf("You answered %d/%d correctly.\n", curr_rounds, num_rounds);
  return 0;
}
