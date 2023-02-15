/*----------------------------------------------
 * Author: Bridge Schaad 
 * Due Date: 2/15
 * Description: danceable.c
 * 		Read and store songlist from csv file. Allow users to easily access
 *		most danceable songs.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    struct song* val;
    struct node* next;
};

struct song {
    char title[128]; 
    char artist[128];
    int minutes;
    int seconds; // milliseconds
    float danceability;
    float energy;
    float tempo;
    float valence;
};


/** 
 * addSong fills a struct song* with data from the char* buff, using comma delineated tokens
 */
void addSong(char* buff, struct song* newSong) {    
    char* token = strtok(buff, ",\n");
    int i = 0; // track tokens
    while (token) {
        i++;
        if (i%7 == 1) {
		strcpy(newSong->title, token);
        }
        else if (i%7 == 2) {
		strcpy(newSong->artist, token);
        }
        else if (i%7 == 3) {
	    newSong->minutes = atoi(token)/60000; // convert milliseconds to minutes
	    newSong->seconds = atoi(token)%60000 / 1000; // convert milliseconds to seconds
	}
	else if (i%7 == 4) {
	    newSong->danceability = atof(token);
	}
	else if (i%7 == 5) {
	    newSong->energy = atof(token);
	}
	else if (i%7 == 6) {
	    newSong->tempo = atof(token);
	}
	else if (i%7 == 0) {
	    newSong->valence = atof(token);
	}
        token = strtok(NULL, ",");
    } 
}

/** addLast adds the newLast to the end of a linked list with header *list */
struct node* addLast(struct node* list, struct node* newLast) {
    struct node* n = list;
    while (n->next != NULL) { // find last node
	n = n->next;
    }
    n->next = newLast;
    n = newLast;
    
    return n;
}

/** printLib prints the songs at each node in the linked list, followed by the number of 
 *  songs in the list.
 */
void printLib(struct node* list) {
    int i = 0; // index
    for (struct node* n = list->next; n != NULL; n = n->next) {
	printf("%2d) %-25s Artist: %-20s (%d:%02ds)  D: %.2f E: %.3f\n    T: %3.3f V: %.3f\n", 
	i, n->val->title, n->val->artist, n->val->minutes, n->val->seconds, n->val->danceability, n->val->energy, n->val->tempo, n->val->valence);
	i++;
    }
    printf("\nDataset contains %d songs.\n\n", i);
}

/** findDance prints, then returns the index of the node with the most danceable song. */
int findDance(struct node* list) {
    // find index of node w/ max danceability
    int i = 0;
    int maxPos; // track position of most danceable
    float max = 0; // track highest danceability rating
    for (struct node* n = list-> next; n != NULL; n = n-> next) {
	if (n->val->danceability > max) {
	    max = n->val->danceability;
	    maxPos = i;
	}
	i++;
    }

    // print node w/ max danceability
    struct node* n = list;
    for (int j = 0; j <= maxPos; j++) {
	n = n->next;
    }
    printf("------------------ Most Danceable ---------------------\n");
    printf("%-25s Artist: %-20s (%d:%02ds)  D: %.2f E: %.3f\n    T: %3.3f V: %.3f\n", n->val->title, 
    n->val->artist, n->val->minutes, n->val->seconds, n->val->danceability, n->val->energy, n->val->tempo, n->val->valence);
    printf("-------------------------------------------------------\n\n");
    return maxPos;
}

int main() {
    // open file
    FILE* infile = NULL;
    // FILE* outfile = NULL;

    infile = fopen("songlist.csv", "r");
    if (infile == NULL) {
	printf("Error: unable to open songlist.");
	exit(1);
    }
    char firstline[256];
    fgets(firstline, 256, infile);
  
    // fill linked list
    struct song* head = NULL;
    struct node headNode = {head, NULL};
    struct node *list = &headNode; 
    char buffer[1024];
    while (fgets(buffer, 1024, infile)) {
	struct node* newNode = malloc(sizeof(struct node));
	newNode -> next = NULL;
	struct song* newSong = malloc(sizeof(struct song));
	addSong(buffer, newSong);
	newNode -> val = newSong;
	addLast(list, newNode);
    }

    // find most danceable
    while (1) {
	printLib(list);
	printf("Press 'd' to show the most danceable song (any other key to quit): \n");
	char entry;
	scanf("%c", &entry);
	if (entry != 'd') {
	    break;
	}
	else {
	    // find most danceable
	    int dancePos = findDance(list);
	    // delete most danceable
	    struct node* n = list;
	    for (int i = 0; i < dancePos; i++) {
		n = n->next;
	    }
	    struct node* tmp = n->next;
	    n->next = tmp->next;
	    free(tmp->val);
	    free(tmp);
	}
    }
 
    // close file and free list
    fclose(infile);
    struct node* n = list;
    while (list!=NULL) {
	n = list;
	list = list->next;
	free(n->val);
	free(n); 
    }
    return 0;
}
