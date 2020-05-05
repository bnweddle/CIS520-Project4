#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_THREADS 4
#define BUFFSIZE 12
//define FILENAME "a.txt"
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int *sums = malloc(BUFFSIZE * sizeof(int));
int count = 0;
int j = 0;


int main()
{
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
       
     for (t = 0; t < MAX_THREADS; t++ ){
        rc = pthread_create(&threads[t], &attr, &FindSums, (void *)t);
        if (rc)
        {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
        }
    }

    /* Free attribute and wait for the other threads */
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
   FindSums(count, j, sums);

      
   fclose(fp);
   return 0;
}

void *FindSums(void *id)
{   
    for(j = 0; j < count; j++) 
    {
	      printf("tid:%d line %d-%d: %d\n",omp_get_thread_num(),j,j+1,(sums[j+1]-sums[j]));
    }
    
    pthread_exit(NULL);
}
