#include <stdio.h>
#include <omp.h>
#define MAX_THREADS 4
#define FILENAME "a.txt"
//define FILENAME "/homes/dan/625/wiki_dump.txt"

// Does the same thing as the test.c should.

int main()
{
    FILE *fp;
    int count = 0;  // Line counter (result)
    char c;  // To store a character read from file
    int lineA = 0;
    int lineB = 0;
    int tid;
    int dif = 0;
    int flag = 0;

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
    #pragma omp parallel
    {
    #pragma omp shared(count,lineA,lineB) private(dif) for schedule(static)
      for (c = getc(fp); c != EOF; c = getc(fp)) {
	tid = omp_get_thread_num();
	if(c != '\n') {
		if(flag==1) {
		   #pragma omp atomic
           	   lineA = lineA + (int)c;
        	} else {
		   #pragma omp atomic
        	   lineB = lineB + (int)c;
        	}
	}
       	if (c == '\n'){ // Increment count if this character is newline
           count++;
           if(flag == 0) {
             flag = 1;
             dif = lineA-lineB;
	     printf("(thread %d) line %d-%d: %d\n",tid,count-1,count,dif);
             lineA = 0;
           }
	   else {
             flag = 0;
             dif = lineB-lineA;
	     printf("(thread %d) line %d-%d: %d\n",tid,count-1,count,dif);
             lineB = 0;
           }
   //          tid = omp_get_thread_num();
 //         //   printf("(thread %d) line %d-%d: %d\n",tid,count-1,count,dif);
        }
      }
    }                             
    // Close the file
    fclose(fp);
    return 0;
}
