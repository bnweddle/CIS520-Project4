
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>
//#define MAX_THREADS 4
#define BUFFSIZE 100 
//define FILENAME "a.txt"
#define FILENAME "/homes/dan/625/wiki_dump.txt"

int MAX_THREADS;
int sums[BUFFSIZE];
int dif[BUFFSIZE];
int temp[BUFFSIZE];
//int count = 0;

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
	 // printf("i:%d sum:%d\n",i,sum);
	  sums[i] = sum;
	 // count++;
	  sum = 0;
       }
      
   fclose(fp);

}

void FindSums(int *rank)
{
    int j = 0;
    int dif;
    int id = *rank - 1;
    int start = id * (BUFFSIZE / MAX_THREADS);
    int end = start + (BUFFSIZE / MAX_THREADS);
    for(j = start; j < end; j++) {
      dif = sums[j+1] - sums[j];
      MPI_Send(&dif, 1, MPI_INT, 0, j, MPI_COMM_WORLD);
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
    
//    printf("size = %d rank = %d\n", numtasks, rank);
//    fflush(stdout);
    
    if(rank == 0){
       ReadFile();
    }
    
    MAX_THREADS = numtasks-1; 
    
    MPI_Bcast(sums, BUFFSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Bcast(dif, BUFFSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank > 0) {
      FindSums(&rank);
    } else {
      int x;
      int rec;
      for(x = 0; x < BUFFSIZE-1; x++) {
        MPI_Recv(&rec, 1, MPI_INT, MPI_ANY_SOURCE, x, MPI_COMM_WORLD, &Status);
        printf("%d-%d: %d\n",x,x+1,rec);
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
   // MPI_Gather(&dif, BUFFSIZE, MPI_INT, temp, BUFFSIZE, MPI_INT, 0, MPI_COMM_WORLD); 

    gettimeofday(&end, NULL);


    //printf("BEFORE:%d\n",dif[0]);
    //MPI_Reduce(sums, dif, BUFFSIZE, MPI_INT, NULL, 0, MPI_COMM_WORLD);
    //printf("AFTER:%d\n",dif[0]);
 
    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize(); 
    
    if(rank == 0){
        int x = 0;
         for(x = 0; x < BUFFSIZE; x++) {
           // printf("%d-%d: %d\n",x,x+1,temp[x]);
         }
      
   	elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0; //sec to ms
    	elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // us to ms
    	printf("DATA, %d, %s, %f, %d\n", myVersion, getenv("SLURM_CPUS_ON_NODE"),  elapsedTime, MAX_THREADS);
     } 
   return 0;
}
