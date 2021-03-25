#!/bin/bash
#SBATCH -p compute
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=128
#SBATCH --mem=4G
#SBATCH -t 03:00:00
#SBATCH -A mor100
#SBATCH -o %x.%j.out
#SBATCH -e %x.%j.err
#SBATCH --export=ALL

echo $(hostname)
taskset -cp $BASHPID

SCRATCH=/scratch/$USER/job_$SLURM_JOB_ID
# copy data from home to scratch
cp $HOME/project01/nbody-p $SCRATCH
cp -r $HOME/project01/examples $SCRATCH


# run the program
echo "Running parallel n-body"

echo "random 25000"
for i in {1..3}
do
    $SCRATCH/nbody-p 0.01 10 1000 $SCRATCH/examples/random100000.npy $SCRATCH/output.npy
done
