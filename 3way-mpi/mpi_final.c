#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>

#define BUFFSIZE 1000000 
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int num_proc;
int sums[BUFFSIZE];

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
	  sums[i] = sum;
	  sum = 0;
       }
      
   fclose(fp);
}

void FindSums(int *rank)
{
     
    int j;
    int id = *rank;
    int start = id * (BUFFSIZE / num_proc);
    int end = start + (BUFFSIZE / num_proc);

    for(j = start; j < end; j++)
    {  
       printf("%d-%d: %d\n",j,j+1,sums[j+1]-sums[j]);    
    }
}


int main(int argc, char* argv[])
{  
    struct timeval start, end;
    double elapsedTime;
    int numSlots, myVersion = 3;
   
    int m, rc;
    int numtasks, rank;
    MPI_Status Status;


    gettimeofday(&start, NULL);
    rc = MPI_Init(&argc, &argv);
    if(rc != MPI_SUCCESS) {
      printf ("Error starting MPI program. Terminating.\n");
      MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    
    if(rank == 0){
       ReadFile();
    }
    
    num_proc = numtasks; 
    
    MPI_Bcast(sums, BUFFSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    FindSums(&rank);
    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Finalize(); 
    if(rank == 0) {
        gettimeofday(&end, NULL);     
   	elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0; //sec to ms
    	elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // us to ms
    	printf("DATA, %d, %s, %f, %d\n", myVersion, getenv("SLURM_CPUS_ON_NODE"),  elapsedTime, num_proc);
    }

    //MPI_Finalize(); 
    return 0;
}
