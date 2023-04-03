/* Author: Bridge Schaad
 * Date: 4/1/2023
 * Description: Uses N threads to search for a value in a data.bin list.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

struct thread_data {
  int start_index;
  int end_index;
  int *array;
  int data_size;
  int search;
  int s_index;
};

void* find_search(void* userdata) {
  struct thread_data *data = (struct thread_data *) userdata;
  int found = 0;
  for (int i = data->start_index; i < data->end_index && i < data->data_size; i++) {
    if (data->array[i] == data->search) {
      found ++;
      data->s_index = i;
      break;
    } 
  }
  return (void*) NULL;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }

  // read in data.bin
  FILE* fp = fopen("data.bin", "rb");
  int size = 0;
  fread(&size, sizeof(int), 1, fp); // read in size
  int* data = malloc(sizeof(int) * size);
  fread(data, sizeof(int), size, fp);

  int search = 0;
  printf("Enter a value to search: ");
  scanf("%d", &search);
  srand(time(0));

  int num_threads = atoi(argv[1]);
  printf("Searching data of size %d with %d threads.\n\n", size, num_threads);
  int subsize = size/num_threads;
  pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
  struct thread_data *t_data = malloc(sizeof(struct thread_data) * num_threads);

  // initialize each thread
  for (int i = 0; i < num_threads; i++) {
    t_data[i].array = data;
    t_data[i].start_index = subsize * i;
    t_data[i].end_index = subsize * (i+1);
    t_data[i].data_size = size;
    t_data[i].search = search;
    t_data[i].s_index = -1;
    pthread_create(&threads[i], NULL, find_search, (void*) &t_data[i]);
  }

  int found = 0;
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
    if (t_data[i].s_index >= 0) { 
      found++;
      printf("Thread %d found %d at index %d.\n", i, search, t_data[i].s_index);
    }
  }
  if (found == 0) {
    printf("Not found!\n");
  }

  fclose(fp);
  free(data);
  free(threads);
  free(t_data);
  return 0;
}
