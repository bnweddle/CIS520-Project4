#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>
//#define MAX_THREADS 4
#define BUFFSIZE 1000 
//define FILENAME "a.txt"
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int MAX_THREADS;
int sums[BUFFSIZE];
int dif[BUFFSIZE];
int difCal[BUFFSIZE];
//int count = 0;

void SetArrayToZero()
{
   int x;
   for(x = 0; x < BUFFSIZE; x++)
      difCal[x] = 1; 
}


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
	  //printf("i:%d sum:%d\n",i,sum);
	  sums[i] = sum;
	 // count++;
	  sum = 0;
       }
      
   fclose(fp);

}

void FindSums(int *rank)
{
     
    int j;
    int id = *rank;
    int start = id * (BUFFSIZE / MAX_THREADS);
    int end = start + (BUFFSIZE / MAX_THREADS);
    // j needs to start where thread left off from
    for(j = start; j < end; j++)
    {  
       dif[j] = sums[j+1]-sums[j];
    
    }
}


int main(int argc, char* argv[])
{  
    struct timeval start, end;
    double elapsedTime;
    int numSlots, myVersion = 3; //pthreads = 1, openmp = 2, mpi = 3 
   
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
    
    printf("size = %d rank = %d\n", numtasks, rank);
    fflush(stdout);
    
    if(rank == 0){
       ReadFile();
       SetArrayToZero();
    }
    
    MAX_THREADS = numtasks; 
    
    MPI_Bcast(sums, BUFFSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(difCal, BUFFSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    FindSums(&rank);

    //SetArrayToZero();
    //printf("BEFORE %d:\n", dif[5]);
    

    MPI_Reduce(dif, difCal, BUFFSIZE, MPI_INT, MPI_PROD, 0 , MPI_COMM_WORLD);
    
    //SetArrayToZero();
    //printf("AFTER %d:\n", dif[5]);

    int x = 0;
         
    for(x = 0; x < BUFFSIZE; x++) {
       if(dif[x] != 0)
         printf("%d-%d: %d\n",x,x+1, dif[x]);
     }

     gettimeofday(&end, NULL);

     MPI_Finalize();
     if(rank == 0) {     
   	elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0; //sec to ms
    	elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // us to ms
    	printf("DATA, %d, %s, %f, %d\n", myVersion, getenv("SLURM_CPUS_ON_NODE"),  elapsedTime, MAX_THREADS);
      } 
   return 0;
}
