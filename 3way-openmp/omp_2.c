#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>
#define MAX_THREADS 2 
#define NUMLINES 1000000
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int main() {
  struct timeval start, end;
  double elapsedTime;
  int numSlots, myVersion = 1; // omp = 1, pthread = 2, mpi = 3
 
 
  FILE * fp;
  int count = 0; // tracks total number of lines read
  char c = 0; // stores the char read from file
  int sum = 0; // the sum of a line's chars
  int * sums = malloc(NUMLINES * sizeof(int)); // a buffer to hold line sums
  int i = 0;
  int j = 0;


  omp_set_num_threads(MAX_THREADS);
  
  // Open the file
  fp = fopen(FILENAME, "r");

  // Check if file exists
  if (fp == NULL) {
    printf("Failed to open file");
    return -1;
  }
  printf("File opened successfully\n");
  
  gettimeofday(&start, NULL);
  // loop which reads characters from the file, stopping when EOF is reached or buffer is full
  while(c = getc(fp), c != EOF && i < NUMLINES) {
    if (c == '\n') {
      sums[i] = sum; // save line sum to sums buffer
      i++;
      sum = 0; // reset value for next line
    } else {
      sum += (int)c; // add each char to line sum
    }
  }
  count += i; // count tracks total number of lines across all loops

  #pragma omp parallel
  {
    #pragma omp for ordered
    for (j = 0; j < count-1; j++) {
      #pragma omp ordered
      printf("(thread %d) line %d-%d: %d\n", omp_get_thread_num(), j, j + 1, (sums[j + 1] - sums[j]));
    }
  }

  gettimeofday(&end, NULL);


  elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0; //sec to ms
  elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // us to ms
  printf("DATA, %d, %s, %f, %d\n", myVersion, getenv("NSLOTS"),  elapsedTime, MAX_THREADS);
  
  fclose(fp);
  return 0;
}
