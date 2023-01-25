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
  int num_rounds = 0; /** num_rounds: number of rounds */
  int corr_rounds = 0; /** corr_rounds: number of correct rounds */

  printf("Welcome to Math Game!\n");
  printf("How many rounds do you want to play? ");
  scanf("%d", &num_rounds);


  int i = 0;
  for (i; i < num_rounds; i++) {
    int given_ans = 0;
    srand(time(NULL)); /** initialize a random number generator */
    int random1 = rand() % 10; /**generate random number 1-10 */
    int random2 = rand() % 10;

    printf("%d + %d = ? ", random1, random2);
    scanf("%d", &given_ans);

    if (given_ans == (random1 + random2)) {
      corr_rounds++;
      printf("Correct!\n");
    }
    else {
      printf("Incorrect :(\n");
    }
  }

  printf("You answered %d/%d correctly.\n", corr_rounds, num_rounds);
  return 0;
}
