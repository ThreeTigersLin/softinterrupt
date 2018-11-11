
/*
 * Michael Recachinas
 * CS 4414: Operating Systems
 * Produce N Page Faults - Challenge
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char ** argv) {
    printf("Page size: %d\n",getpagesize());
    printf("Int size: %lu\n", sizeof(int));
    if (argc < 2){
        printf("Usage: ./page_fault <number of page faults>\n");
        return -1;
    }
    int i,j,N, page_size,int_size; /* Declare variables */
    N = atoi(argv[1]); /* Grab approximate number of desired page faults */
    int_size = sizeof(int); /* Get machine's size of representing signed integers */
    page_size = (int)getpagesize()/int_size; /* Entries per page for signed integers */
    int a[N]; /* 2D Array where the ROWS are the number of desired faults and the COLS have the size of a page */
    int x = 0;

    clock_t start = clock();
 //   x += a[0][0];

    for (i = 0; i < 1; i++) {
        for (j=0; j<N; j++) {
            // printf("a[%d][%d]: %d\n", j, i, a[j][i]);
             a[j][i] = 1;
        }
    }

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000000;
    printf("Time consumed = %.4f; page_fault = %.4f; x = %d;\n", cpu_time_used, cpu_time_used / N, x);

    return 0;
}
