#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#define MAX_THREADS 4
#define BUFFSIZE 12
//define FILENAME "a.txt"
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int main()
{
    FILE *fp;
    int count = 0;  // Line counter (result)
    char c = 0;  // To store a character read from file
    int sum = 0;
    int *sums = malloc(BUFFSIZE * sizeof(int)); 
    int i = 0;
    int j = 0;
    int tid = 0; 
    int start = 0;
    int end = 0;

    omp_set_num_threads(MAX_THREADS);

    // Open the file
    fp = fopen(FILENAME, "r");

    // Check if file exists
    if (fp == NULL)
    {
       printf("Could not open file"); // breaks here
       return 0;
    }

    // Extract characters from file and store in character c
       for(i=0; i <= BUFFSIZE; i++) {
	  c = getc(fp);
	  while(c != '\n') {
            if(c == EOF) {
              break;
            }
	    sum += (int)c;
	    c = getc(fp);
          }
	  printf("i:%d sum:%d\n",i,sum);
	  sums[i] = sum;
	  count++;
	  sum = 0;
       }


       #pragma omp parallel private(tid, start, end)
       {
          tid = omp_get_thread_num();
          start = tid * (BUFFSIZE / MAX_THREADS);
          end = start + (BUFFSIZE / MAX_THREADS);
       #pragma omp for ordered
       for(j = 0; j < count; j++) {
	#pragma omp ordered
	  printf("tid:%d line %d-%d: %d\n",omp_get_thread_num(),j,j+1,(sums[j+1]-sums[j]));
       }
       }
fclose(fp);
return 0;
}
