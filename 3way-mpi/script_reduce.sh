#!/bin/sh
$SBATCH --output=mpi_4.out
srun mpirun -np 4 ./mpi_reduce
