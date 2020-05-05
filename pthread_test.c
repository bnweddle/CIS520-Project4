#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_THREADS 10
#define BUFFSIZE 100
//define FILENAME "a.txt"
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int sums[BUFFSIZE];
int count = 0;
int j = 0;


void* FindSums(void *arg)
{
    int id = (int)(arg);
    int start = id * ( BUFFSIZE / MAX_THREADS);
    int end = start + (BUFFSIZE /MAX_THREADS);

    
    // j needs to start where thread left off from
    for(j = start; j < end; j++)
    {
        printf("tid-%d  line %d-%d: %d\n", pthread_self(), j,j+1,(sums[j+1]-sums[j]));
    }

    pthread_exit(NULL);

}


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
        rc = pthread_create(&threads[t], NULL, FindSums, (void *)t);
        if (rc)
        {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
        }
    }

    for(t=0; t< MAX_THREADS; t++)
    {
      rc = pthread_join(threads[t], NULL);
      if (rc)
      {
        printf("ERROR; return code from pthread_join() is %d\n", rc);
        exit(-1);
      }
    }


      
   fclose(fp);
   return 0;
}
