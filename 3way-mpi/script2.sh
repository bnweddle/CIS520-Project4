#!/bin/sh
#SBATCH --output=mpi2.out
srun -n 2 ./mpi
