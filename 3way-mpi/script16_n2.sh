#!/bin/sh
#SBATCH --output=mpi16_n2.out
srun -n 16 ./mpi
