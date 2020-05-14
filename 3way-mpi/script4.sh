#!/bin/sh
#SBATCH --output=mpi4.out
srun -n 4 ./mpi
