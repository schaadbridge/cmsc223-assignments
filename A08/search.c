/**
 * Author: Bridge Schaad
 * Date: 3/28/2023
 * Description: Uses two processes to search for a value in a list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // read in data.bin
    FILE* fp = fopen("data.bin", "rb");
    int size = 0;
    fread(&size, sizeof(int), 1, fp); // read in size
    int* data = malloc(sizeof(int) * size);
    fread(data, sizeof(int), size, fp);
    
    int search = 0;
    printf("Enter a value to search: ");
    scanf("%d", &search);
    
    // fork
    pid_t pid;
    pid = fork();
    if (search < 0) {
        printf("Not found!\n");
        exit(1);
    }
    if (pid) { // parent
        int found = 0;
        for (int i = size/2; i < size; i++) {
            if (data[i] == search) {
                found ++;
                printf("Parent found %d at index %d.\n", search, i);
                break;
            }
        }
        if (found == 0) {
            printf("Not found!\n");
        }
    }
    else { // child
        for (int i = 0; i < size/2; i++) {
            if (data[i] == search) {
                printf("Child found %d at index %d.\n", search, i);
                exit(1);
            }
        }
        printf("Not found!\n");
    }
    
    fclose(fp);
    free(data);
    return 0;
}
