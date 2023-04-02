/* Author: Bridge Schaad
 * Date: 4/2/2023
 * Description: Using 4 threads, compute a program that outputs a PPM image 
 * of the mandelbrot set.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct thread_data {
  float t_xmin;
  float t_xmax;
  float t_ymin;
  float t_ymax;
  struct ppm_pixel* array;
  struct ppm_pixel* colors;
  int xsmall;
  int xlarge;
  int ysmall;
  int ylarge;
  int t_maxIterations;
  int t_size;
};

void* fill_pix(void* userdata) {
  struct thread_data *data = (struct thread_data*) userdata;
  for (double i = data->xsmall; i < data->xlarge; i++) { // for each row
    for (double j = data->ysmall; j < data->ylarge; j++) { // for each column
      double xfrac = i / (double)data->t_size;
      double yfrac = j / (double)data->t_size;
      double x0 = data->t_xmin + xfrac * (data->t_xmax - data->t_xmin);
      double y0 = data->t_ymin + yfrac * (data->t_ymax - data->t_ymin);
      int idx = j * data->t_size + i;

      double x = 0;
      double y = 0;
      int iter = 0;
      while ((iter < data->t_maxIterations) && ((x*x + y*y) < 4)) {
        double xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < data->t_maxIterations) {
        data->array[idx] = data->colors[iter];
      }
      else {
        data->array[idx].red = 0;
        data->array[idx].green = 0;
        data->array[idx].blue = 0;
      }
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
	// generate pallet
  srand(time(NULL));
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 225;
    palette[i].green = rand() % 225;
    palette[i].blue = rand() % 225;
  }

  // compute image
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  struct ppm_pixel* mandelbrot = malloc(sizeof(struct ppm_pixel) * size * size    );
  pthread_t *threads = malloc(sizeof(pthread_t) * numThreads);
  struct thread_data *data = malloc(sizeof(struct thread_data) * numThreads);
  for (int i = 0; i < numThreads; i++) {
    data[i].t_xmin = xmin;
    data[i].t_xmax = xmax;
    data[i].t_ymin = ymin;
    data[i].t_ymax = ymax;
    data[i].xsmall = size/2 * (i%2);
    data[i].xlarge = (size * ((i%2) + 1)) / 2;
    data[i].ysmall = size * (i/2) / 2;
    data[i].ylarge = (size * ((i/2) + 1)) / 2;
    data[i].colors = palette;
    data[i].array = mandelbrot;
    data[i].t_maxIterations = maxIterations;
    data[i].t_size = size;
    printf("Thread %ld) sub-image block: cols (%d, %d) to rows (%d, %d)\n", 
          threads[i], data[i].xsmall, data[i].xlarge, data[i].ysmall, 
          data[i].ylarge);
    pthread_create(&threads[i], NULL, fill_pix, &data[i]);
  }

  for (int i = 0; i < numThreads; i++) {
    printf("Thread %ld finished.\n", threads[i]);
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  double time_diff = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - 
                      tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g seconds.\n", size, size, 
          time_diff);
  
  // write array into file
  time_t timestamp = time(0);
  char new_file[40] = "";
  sprintf(new_file, "mandlebrot-%d-%li.ppm", size, timestamp);
  printf("Writing file: %s\n", new_file);
  write_ppm(new_file, mandelbrot, size, size);

  free(palette);
  free(mandelbrot);
  free(threads);
  free(data);
}
