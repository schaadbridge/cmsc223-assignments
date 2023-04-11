/* Author: Bridge Schaad
 * Date: 4/10/2023
 * Description: Outputs a PPM image of the buddhabrot set using multiple threads.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

pthread_mutex_t mutex;
pthread_barrier_t barrier;
struct thread_data {
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int* count;
  int maxcount;
  int xsmall;
  int xlarge;
  int ysmall;
  int ylarge;
  int maxIterations;
  struct ppm_pixel* array;
  int size;
};

void* fill_pix(void* userdata) {
  struct thread_data *data = (struct thread_data*) userdata;
  int* membership = malloc (sizeof(int) * data->size * data->size);
  // step 1: determine mandelbrot set membership
  for (double i = data->xsmall; i < data->xlarge; i++) { // for each row
    for (double j = data->ysmall; j < data->ylarge; j++) { // for each column
      double xfrac = i / (double)data->size;
      double yfrac = j / (double)data->size;
      double x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      double y0 = data->ymin + yfrac * (data->ymax - data->ymin);
      int idx = j * data->size + i;

      double x = 0;
      double y = 0;
      int iter = 0;
      while (iter < data->maxIterations && ((x*x + y*y) < 4)) {
        double xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < data->maxIterations) {
        membership[idx] = 0;
      }
      else {
        membership[idx] = 1;
      }
    }
  }

  // step 2: compute visited counts
  for (double i = data->xsmall; i < data->xlarge; i++) { // for each row
    for (double j = data->ysmall; j < data->ylarge; j++) { // for each column
      int idx = j * data->size + i;
      if (membership[idx] == 0) {
        double xfrac = i / (double)data->size;
        double yfrac = j / (double)data->size;
        double x0 = data->xmin + xfrac * (data->xmax - data->xmin);
        double y0 = data->ymin + yfrac * (data->ymax - data->ymin);
        int idx = j * data->size + i;

        double x = 0;
        double y = 0;
        while ((x*x + y*y) < 4) {
          double xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;

          int yrow = round(data->size * (y - data->ymin) / (data->ymax - data->ymin));
          int xcol = round(data->size * (x - data->xmin) / (data->xmax - data->xmin));
          int count_idx = yrow * data->size + xcol;
          if (yrow < 0 || yrow >= data->size) { continue;}
          if (xcol < 0 || xcol >= data->size) { continue;}    
           
          pthread_mutex_lock(&mutex);
          data->count[count_idx] ++;
          if (data->count[count_idx] > data->maxcount) { // update maxcounts
            data->maxcount = data->count[count_idx];
          }
          pthread_mutex_unlock(&mutex);
        }
      }
    }
  }

  // step 3: compute colors
  pthread_barrier_wait(&barrier);
  double gamma = 0.681;
  double factor = 1.0/gamma;
  for (double i = data->xsmall; i < data->xlarge; i++) { // for each row
    for (double j = data->ysmall; j < data->ylarge; j++) { // for each column
      double value = 0;
      int idx = j * data->size + i;
      struct ppm_pixel color;
      
      if (data->count[idx] > 0) {
        value = log(data->count[idx]) / log(data->maxcount);
        value = pow(value, factor); 
      }

      color.red = value * 225;
      color.green = value * 255;
      color.blue = value * 255;
      data->array[idx] = color;
    }
  }
 
  return (void *) NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numThreads = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
                    "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // generate canvas
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  struct ppm_pixel* mandelbrot = malloc(sizeof(struct ppm_pixel) * size * size);
  pthread_t *threads = malloc(sizeof(pthread_t) * numThreads);
  struct thread_data *data = malloc(sizeof(struct thread_data) * numThreads);
  int* counts = malloc(sizeof(int) * size * size); 
  for (int i = 0; i < size * size; i++) {
    counts[i] = 0;    
  }
  int maxcount = 0;

  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, numThreads);

  for (int i = 0; i < numThreads; i++) {
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].xsmall = size/2 * (i%2);
    data[i].xlarge = (size * ((i%2) + 1)) / 2;
    data[i].ysmall = size * (i/2) / 2;
    data[i].ylarge = (size * ((i/2) + 1)) / 2;
    data[i].array = mandelbrot;
    data[i].maxIterations = maxIterations;
    data[i].size = size;
    data[i].count = counts;
    data[i].maxcount = maxcount;
    pthread_create(&threads[i], NULL, fill_pix, &data[i]);
    printf("Thread %ld) sub-image block: cols (%d, %d) to rows (%d, %d)\n", 
        threads[i], data[i].xsmall, data[i].xlarge, data[i].ysmall, 
        data[i].ylarge);
  }

  for (int i = 0; i < numThreads; i++) {
    printf("Thread %ld finished.\n", threads[i]);
    pthread_join(threads[i], NULL);
  }
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);

  gettimeofday(&tend, NULL);
  double time_diff = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - 
      tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g seconds.\n", size, size, 
      time_diff);

  // write array into file
  time_t timestamp = time(0);
  char new_file[40] = "";
  sprintf(new_file, "buddhabrot-%d-%li.ppm", size, timestamp);
  printf("Writing file: %s\n", new_file);
  write_ppm(new_file, mandelbrot, size, size);

  free(mandelbrot);
  free(threads);
  free(data);
  free(counts);
}
