/*----------------------------------------------
 * Author: Bridge
 * Date: Due 2/8/2023
 * Description: Read CSV file contents into an array. Print list of songs.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct song {
	char artist[128];
	char title[128];
	int minutes;
	int seconds; // milliseconds
	float danceability;
	float energy;
	float tempo;
	float valence;
};

int main() {
	// open file
	FILE* infile = NULL;
	// FILE* outfile = NULL;

	infile = fopen("songlist.csv", "r");
	if (infile == NULL) {
		printf("Error: unable to open songlist.");
		exit(1);
	}
	// make dynamic array of struct song
	struct song* dynamicList;
	char firstLine[32];
	fgets(firstLine, 32, infile);
	int length = atoi(strtok(firstLine, ",")); // # of songs is first entry in csv
	dynamicList = malloc(sizeof(struct song) * length);

	// skip rest of line and line 2
	char secondLine[256];
	fgets(secondLine, 256, infile);
	// read each line into array
	char buffer[1024];
	int i = 0; // count tokens
	int j = 0; // count lines
	while (fgets(buffer, 1024, infile)) { // for each line
		struct song temp;
		char* token = strtok(buffer, ",\n");
		while (token) {
			i++;
			if (i%7 == 1) {
				strcpy(temp.title, token);
			}
			if (i%7 == 2) {
				strcpy(temp.artist, token);
			}
			if (i%7 == 3) {
				temp.minutes = atoi(token)/60000; // convert milliseconds to minutes
				temp.seconds = atoi(token)%60000 / 1000; // convert milliseconds to seconds
			}
			if (i%7 == 4) {
				temp.danceability = atof(token);
			}
			if (i%7 == 5) {
				temp.energy = atof(token);
			}
			if (i%7 == 6) {
				temp.tempo = atof(token);
			}
			if (i%7 == 0) {
				temp.valence = atof(token);
			}
			token = strtok(NULL, ",");
		}
		dynamicList[j] = temp;
		j++;
	}
	// print library
	printf("Welcome to the dynamic song list!\n\n");
	for (int k = 0; k < j; k++) {
		printf("%2d) %-25s Artist: %-20s Duration: %d:%02d  D: %.2f E: %.3f \n    T: %.3f V: %.3f\n", 
		k, dynamicList[k].title, dynamicList[k].artist, dynamicList[k].minutes, dynamicList[k].seconds,
		dynamicList[k].danceability, dynamicList[k].energy, dynamicList[k].tempo, dynamicList[k].valence);
	}
	// free dynamic array and close file
	free(dynamicList);
	fclose(infile);
  return 0;
}
