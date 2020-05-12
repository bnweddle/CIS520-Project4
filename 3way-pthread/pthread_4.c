#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <pthread.h>
#define MAX_THREADS 4 
#define BUFFSIZE 1000000 
//#define FILENAME "a.txt"
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int sums[BUFFSIZE];
int count = 0;

void *FindSums(void *arg)
{   
    int j;
    int id = (uintptr_t)arg;
    int start = id  * (BUFFSIZE / MAX_THREADS);
    int end = start + (BUFFSIZE /MAX_THREADS);

    
    // j needs to start where thread left off from
    for(j = start; j <  end; j++)
    {
        printf("tid-%d  line %d-%d: %d\n", pthread_self(), j,j+1,(sums[j+1]-sums[j]));
       // sleep(0.001);
    }

    pthread_exit((void*) arg);

}


int main()
{
    struct timeval start, end;
    double elapsedTime;
    int numSlots, myVersion = 2; // pthreads = 1, openmp = 2, mpi = 3
    
    FILE *fp;
   // int count = 0;  // Line counter (result)
    char c = 0;  // To store a character read from file
    int sum = 0;
   // int *sums = malloc(BUFFSIZE * sizeof(int)); 
    int i = 0;
   // int j = 0;
    
    int t, rc;
    pthread_t threads[MAX_THREADS];
    pthread_attr_t attr;
    void *status;
    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    
    gettimeofday(&start, NULL);
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
	 // printf("line:%d sum:%d\n",i,sum);
	  sums[i] = sum;
	  count++;
	  sum = 0;
       }
       
     for (t = 0; t < MAX_THREADS; t++ ){
        rc = pthread_create(&threads[t], &attr, FindSums, (void *)(uintptr_t)t);
        if (rc)
        {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
        }
    }

    pthread_attr_destroy(&attr);
    for(t=0; t< MAX_THREADS; t++)
    {
      rc = pthread_join(threads[t], &status);
      if (rc)
      {
        printf("ERROR; return code from pthread_join() is %d\n", rc);
        exit(-1);
      }
    } 

   gettimeofday(&end, NULL);

   elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0; //sec to ms
   elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // us to ms
   printf("DATA, %d, %s, %f, %d\n", myVersion, getenv("NSLOTS"),  elapsedTime, MAX_THREADS );

   fclose(fp);
   return 0;
}
