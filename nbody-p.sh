#!/bin/bash
#SBATCH -p compute
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
cp $HOME/project01/nbody-p $SCRATCH
cp $HOME/project01/nbody-p3 $SCRATCH
cp -r $HOME/project01/examples $SCRATCH


# run the programs
echo "Running parallel n-body"

echo "random 25"
for i in {1..3}
do
    $SCRATCH/nbody-p 0.01 10 1000 $SCRATCH/examples/random25.npy $SCRATCH/output.npy
done
echo "random 100"
for i in {1..3}
do
    $SCRATCH/nbody-p 0.01 10 1000 $SCRATCH/examples/random100.npy $SCRATCH/output.npy
done
echo "random 250"
for i in {1..3}
do
    $SCRATCH/nbody-p 0.01 10 1000 $SCRATCH/examples/random250.npy $SCRATCH/output.npy
done
echo "random 500"
for i in {1..3}
do
    $SCRATCH/nbody-p 0.01 10 1000 $SCRATCH/examples/random500.npy $SCRATCH/output.npy
done
echo "random 1000"
for i in {1..3}
do
    $SCRATCH/nbody-p 0.01 10 1000 $SCRATCH/examples/random1000.npy $SCRATCH/output.npy
done
echo "random 10000"
for i in {1..3}
do
    $SCRATCH/nbody-p 0.01 10 1000 $SCRATCH/examples/random10000.npy $SCRATCH/output.npy
done


echo "Running parallel newton's 3rd law n-body"

echo "random 25"
for i in {1..3}
do
    $SCRATCH/nbody-p3 0.01 10 1000 $SCRATCH/examples/random25.npy $SCRATCH/output.npy
done
echo "random 100"
for i in {1..3}
do
    $SCRATCH/nbody-p3 0.01 10 1000 $SCRATCH/examples/random100.npy $SCRATCH/output.npy
done
echo "random 250"
for i in {1..3}
do
    $SCRATCH/nbody-p3 0.01 10 1000 $SCRATCH/examples/random250.npy $SCRATCH/output.npy
done
echo "random 500"
for i in {1..3}
do
    $SCRATCH/nbody-p3 0.01 10 1000 $SCRATCH/examples/random500.npy $SCRATCH/output.npy
done
echo "random 1000"
for i in {1..3}
do
    $SCRATCH/nbody-p3 0.01 10 1000 $SCRATCH/examples/random1000.npy $SCRATCH/output.npy
done
echo "random 10000"
for i in {1..3}
do
    $SCRATCH/nbody-p3 0.01 10 1000 $SCRATCH/examples/random10000.npy $SCRATCH/output.npy
done




echo "Running parallel n-body: random 25000"
for i in {1..3}
do
    $SCRATCH/nbody-p 0.01 10 1000 $SCRATCH/examples/random25000.npy $SCRATCH/output.npy
done

echo "Running parallel newton's 3rd law n-body: random 25000"
for i in {1..3}
do
    $SCRATCH/nbody-p3 0.01 10 1000 $SCRATCH/examples/random25000.npy $SCRATCH/output.npy
done