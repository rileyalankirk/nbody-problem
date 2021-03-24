#!/bin/bash
#SBATCH -p shared
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=128
#SBATCH --mem=4G
#SBATCH -t 24:00:00
#SBATCH -A mor100
#SBATCH -o %x.%j.out
#SBATCH -e %x.%j.err
#SBATCH --export=ALL

echo $(hostname)
taskset -cp $BASHPID

SCRATCH=/scratch/$USER/job_$SLURM_JOB_ID
# copy data from home to scratch
cp $HOME/project01 $SCRATCH
# enter the project01 folder
cd project01



# run the programs

echo "Running serial n-body"

echo "random 100"
for i in {1..3}
do ./nbody-s 0.01 10 1000 $SCRATCH/random100.npy $SCRATCH/output.npy
done
echo "random 1000"
for i in {1..3}
do ./nbody-s 0.01 10 1000 $SCRATCH/random1000.npy $SCRATCH/output.npy
done
echo "random 10000"
for i in {1..3}
do ./nbody-s 0.01 10 1000 $SCRATCH/random10000.npy $SCRATCH/output.npy
done


echo "Running serial newton's 3rd law n-body"

echo "random 100"
for i in {1..3}
do ./nbody-s3 0.01 10 1000 $SCRATCH/random100.npy $SCRATCH/output.npy
done
echo "random 1000"
for i in {1..3}
do ./nbody-s3 0.01 10 1000 $SCRATCH/random1000.npy $SCRATCH/output.npy
done
echo "random 10000"
for i in {1..3}
do ./nbody-s3 0.01 10 1000 $SCRATCH/random10000.npy $SCRATCH/output.npy
done


echo "Running parallel n-body"

echo "random 100"
for i in {1..3}
do ./nbody-p 0.01 10 1000 $SCRATCH/random100.npy $SCRATCH/output.npy
done
echo "random 1000"
for i in {1..3}
do ./nbody-p 0.01 10 1000 $SCRATCH/random1000.npy $SCRATCH/output.npy
done
echo "random 10000"
for i in {1..3}
do ./nbody-p 0.01 10 1000 $SCRATCH/random10000.npy $SCRATCH/output.npy
done


echo "Running parallel newton's 3rd law n-body"

echo "random 100"
for i in {1..3}
do ./nbody-p3 0.01 10 1000 $SCRATCH/random100.npy $SCRATCH/output.npy
done
echo "random 1000"
for i in {1..3}
do ./nbody-p3 0.01 10 1000 $SCRATCH/random1000.npy $SCRATCH/output.npy
done
echo "random 10000"
for i in {1..3}
do ./nbody-p3 0.01 10 1000 $SCRATCH/random10000.npy $SCRATCH/output.npy
done
