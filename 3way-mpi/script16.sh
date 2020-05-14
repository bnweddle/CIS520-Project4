#!/bin/sh
#SBATCH --output=mpi16.out
srun -n 16 ./mpi
