/** 
 * Author: Bridge Schaad
 * Date: 3/20/2023
 * Description: Output a PPM image of the mandelbrot set in the given parameters.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
    int size = 480;
    float xmin = -2.0;
    float xmax = 0.47;
    float ymin = -1.12;
    float ymax = 1.12;
    int maxIterations = 1000;

    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
        }
    }
    printf("Generating mandelbrot with size %dx%d\n", size, size);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    // todo: your work here
    // generate pallet
    srand(time(NULL));
    struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
    for (int i = 0; i < maxIterations; i++) {
        palette[i].red = rand() % 225;
        palette[i].green = rand() % 225;
        palette[i].blue = rand() % 225;
    }

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    // compute image
    struct ppm_pixel* mandelbrot = malloc(sizeof(struct ppm_pixel) * size * size);
    for (double i = 0; i < size; i++) { // for each row
        for (double j = 0; j < size; j++) { // for each column
            double xfrac = i / (double)size;
            double yfrac = j / (double)size;
            double x0 = xmin + xfrac * (xmax - xmin);
            double y0 = ymin + yfrac * (ymax - ymin);
            int idx = j * size + i;

            double x = 0;
            double y = 0;
            int iter = 0;
            while ((iter < maxIterations) && ((x*x + y*y) < 4)) {
                double xtmp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtmp;
                iter++;
            }

            if (iter < maxIterations) {
                mandelbrot[idx] = palette[iter];
            }
            else {
                mandelbrot[idx].red = 0;
                mandelbrot[idx].green = 0;
                mandelbrot[idx].blue = 0;
            }    
        }
    }

    gettimeofday(&tend, NULL);
    double time_diff = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Computed mandelbrot set (%dx%d) in %g seconds.\n", size, size, time_diff);
    
    // write array into file
    time_t timestamp = time(0);
    char new_file[40] = "";
    sprintf(new_file, "mandlebrot-%d-%li.ppm", size, timestamp);
    printf("Writing file: %s\n", new_file);
    write_ppm(new_file, mandelbrot, size, size);

    free(palette);
    free(mandelbrot);
}
