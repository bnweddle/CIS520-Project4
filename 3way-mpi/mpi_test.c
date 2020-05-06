#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
//#define MAX_THREADS 4
#define BUFFSIZE 12 
//define FILENAME "a.txt"
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int MAX_THREADS;
int sums[BUFFSIZE];
int count = 0;

void ReadFile()
{
    FILE *fp;
    int i = 0;
    int sum = 0;
    char c = 0;
   
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

void FindSums(int *rank)
{
    int j;
    int dif = BUFFSIZE / MAX_THREADS;
    printf("difference %d:\n", dif);
    int id = *rank;
    int start = id * (BUFFSIZE / MAX_THREADS);
    int end = start + (BUFFSIZE / MAX_THREADS);
    printf("Before for loop\n");
    // j needs to start where thread left off from
    for(j = start; j < end; j++)
    {
        printf("tid-%d  line %d-%d: %d\n", id, j,j+1,(sums[j+1]-sums[j]));
    }
   
    printf("After for loop\n: start-%d\n, end-%d\n, rank%d\n, BUFFSIZE%d\n, MAX_THREADS%d\n", start, end, id, BUFFSIZE, MAX_THREADS);
}


int main(int argc, char* argv[])
{  
    int m, rc;
    int numtasks, rank;
    MPI_Status Status;

    rc = MPI_Init(&argc,&argv);
    if (rc != MPI_SUCCESS) {
      printf ("Error starting MPI program. Terminating.\n");
      MPI_Abort(MPI_COMM_WORLD, rc);
    }

    printf("rank before MPI CALL: %d\n", rank);

    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    
    printf("size = %d rank = %d\n", numtasks, rank);
    fflush(stdout);
    
    if(rank == 0){
       ReadFile();
    }

    fflush(stdout);
    
    MAX_THREADS = numtasks;
    printf("MAX_THREADS: %d\n", MAX_THREADS);
    
    
    printf("Reached B CAST\n");
    MPI_Bcast(sums, BUFFSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Reached over BCAST\n");
    FindSums(&rank);

    int size = BUFFSIZE;

    for(m = 0; m < numtasks; m++)
    {
      MPI_Send(&size, 1, MPI_CHAR, m, BUFFSIZE, MPI_COMM_WORLD); 
    }
   
    //I_Send(BUFFSIZE, numtasks, MPI_CHAR,&rank,0, MPI_COMM_WORLD);
    // Need to call MPI_Reduce?i
    //MPI_Reduce(sums, sums, BUFFSIZE, MPI_CHAR, MPI_SUM, 0, MPI_COMM_WORLD);   

 
    MPI_Finalize(); 
    return 0;
}
