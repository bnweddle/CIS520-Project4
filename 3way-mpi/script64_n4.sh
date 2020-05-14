#!/bin/sh
#SBATCH --output=mpi64_n4.out
srun -n 64 ./mpi
