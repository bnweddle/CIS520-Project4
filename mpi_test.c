#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#define MAX_THREADS 4
#define BUFFSIZE 12
//define FILENAME "a.txt"
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int sums[BUFFSIZE];
int count = 0;
int j = 0;


void ReadFile()
{
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
      
   fclose(fp);
}

void FindSums(void *rank)
{
    int id = (int)(rank);
    int start = id * ( BUFFSIZE / MAX_THREADS);
    int end = start + (BUFFSIZE /MAX_THREADS);
    
    // j needs to start where thread left off from
    for(j = start; j < end; j++)
    {
        printf("tid-%d  line %d-%d: %d\n", pthread_self(), j,j+1,(sums[j+1]-sums[j]));
    }
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
    
    int m, rc;
	  int numtasks, rank;
	  MPI_Status Status;

    rc = MPI_Init(&argc,&argv);
	  if (rc != MPI_SUCCESS) {
	  printf ("Error starting MPI program. Terminating.\n");
          MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	  NUM_THREADS = numtasks;
    
    printf("size = %d rank = %d\n", numtasks, rank);
	  fflush(stdout);
    
    if(rank == 0){
         read_file();
    }
    
    MPI_Bcast(sums, BUFFSIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    FindSums(&rank);
    
    // Need to call MPI_Reduce?
    
    MPI_Finalize();
   
   return 0;
}
