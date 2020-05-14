#!/bin/sh
#SBATCH --output=mpi32_n2.out
srun -n 32 ./mpi
