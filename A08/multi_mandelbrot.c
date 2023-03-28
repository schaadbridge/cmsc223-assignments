#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
    int size = 480;
    float xmin = -2.0;
    float xmax = 0.47;
    float ymin = -1.12;
    float ymax = 1.12;
    int maxIterations = 1000;
    int numProcesses = 4;

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
    printf("  Num processes = %d\n", numProcesses);
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

    int shmid;
    shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size * size, 0644 | IPC_CREAT);
    if (shmid == -1) {
        perror("Error: Cannot initialize shared memory.\n");
        exit(1);
    }
    
    
    struct ppm_pixel* mandelbrot = shmat(shmid, NULL, 0);
    if (mandelbrot == (void*) -1) {
        perror("Error: Cannot access shared memory.\n");
        exit(1);
    }

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    for (int k = 0; k < numProcesses; k++){
        int pid = fork();
        if (pid == 0) {    
            int xsmall = size/2 * (k%2); 
            int xlarge = (size * ((k%2)+1)) / 2;
            int ysmall = size * (k/2) / 2;
            int ylarge = (size * ((k/2)+1)) / 2;
            for (double i = xsmall; i < xlarge; i++) { // for each row
                for (double j = ysmall; j < ylarge; j++) { // for each column
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
            printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", pid, xsmall, xlarge, ysmall, ylarge);
            exit(0);
        }
        else {
            printf("Launched child process: %d\n", pid);
        }
    }
    for (int i = 0; i < numProcesses; i++) {
        int status;
        int pid = wait(&status);
        printf("Child process complete: %d\n", pid);
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
    if (shmdt(mandelbrot) == -1) {
        perror("Error: Cannot detatch from shared memory.\n");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("Error: Cannot remove shared memory.\n");
        exit(1);
    }
    return 0;
}
