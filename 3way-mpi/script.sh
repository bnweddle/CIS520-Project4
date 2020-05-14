#!/bin/sh
#SBATCH --output=mpi.out
srun -n 4 ./mpi
