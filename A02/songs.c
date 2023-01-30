/**
 *  Songs.c
 *  Fills an array with 3 songs and allows user to edit data in array.
 *  Mostly struct practice.
 *  Created by: Bridge
 *  Due: 2/1/2023
*/

#include <stdio.h>
#include <string.h>

struct song {
	 char artist[32];
	 char title[32];
	 int minutes;
	 int seconds;
	 float dancability;
};

int main() {
	struct song songList[3];

	// initialize 3 songs in songList
	strcpy(songList[0].artist, "Bedouine");
	strcpy(songList[0].title, "One of Those Days");
	songList[0].minutes = 2;
	songList[0].seconds = 58;
	songList[0].dancability = 0.41;

	strcpy(songList[1].artist, "Amine");
	strcpy(songList[1].title, "Blinds");
	songList[1].minutes = 1;
	songList[1].seconds = 26;
	songList[1].dancability = 0.86;

	strcpy(songList[2].artist, "Ethel Cain");
	strcpy(songList[2].title, "Sun Bleached Flies");
	songList[2].minutes = 7;
	songList[2].seconds = 35;
	songList[2].dancability = 0.2;

	// Open program intro.
	printf("Welcome to Bridge's Song List.\n");

	while (1) {
		printf("\n");
		// print library
		for (int i = 0; i < 3; i++) {
			struct song temp = songList[i];
			printf("%d) %-20s Artist: %-20s Duration: %d:%02d  Danceability: %.2f\n", i, temp.
			title, temp.artist, temp.minutes, temp.seconds, temp.dancability);
		}
		printf("================");

		printf("\nEnter a song id to edit [0,1,2]: ");
		int val;
		scanf("%d",&val);
		if (val != 0 && val != 1 && val != 2) {
			printf("Invalid choice!\n");
			break;
		}
		else {
			struct song *temp = &songList[val]; 
			printf("Title: %s  Artist:%s\n\n", temp->title, temp->artist);
		 	printf("Which attribute do you wish to edit? [artist, title, duration, dancability]: ");
		 	char attr[32];
		 	scanf("%s", attr);
		 	if (strcmp(attr, "artist") == 0) {
				printf("Enter artist: ");
				char buff[32]; // temporarily hold input
				scanf("%*c%[^\n]%*c", buff); // skip newline from being read as first char
				strcpy((*temp).artist, buff);
		 	}
		 	else if (strcmp(attr, "title") == 0) {
				printf("Enter title: ");
				char buff[32];
				scanf("%*c%[^\n]%*c", buff);
				strcpy((*temp).title, buff);
		 	}
		 	else if (strcmp(attr, "duration") == 0) {
				printf("Enter duration (minutes): ");
				scanf("%d", &(*temp).minutes);
				printf("Enter duration (seconds) [0-59]: ");
				scanf("%d", &(*temp).seconds);
		 	}
		 	else if (strcmp(attr, "dancability") == 0) {
				printf("Enter dancability[0-1.0]: ");
				scanf("%f", &(*temp).dancability);
		 	}
		 	else {
		 		printf("Invalid choice!\n");
		 		break;
		 	}
		}
	}
	return 0;
}
