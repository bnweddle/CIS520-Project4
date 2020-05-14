#!/bin/sh
#SBATCH --output=mpi8.out
srun -n 8 ./mpi
