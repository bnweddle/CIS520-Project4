#!/bin/sh
#SBATCH --output=mpi1.out
srun -n 1 ./mpi
