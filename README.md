# Local Machine Compiling and Running

# PLUTO CHARON
# Serial 3:
gcc -Wall -O3 -march=native nbody-s3.c nbody-h.c matrix.c util.c -o nbody-s3 -lm
./nbody-s3 1 302400 10000 examples/pluto-charon.npy output.npy

# Parallel 3:
gcc-10 -fopenmp -Wall -O3 -march=native nbody-p3.c nbody-h.c matrix.c util.c -o nbody-p3 -lpthread -lm
./nbody-p3 1 302400 1000 examples/pluto-charon.npy output.npy

# Gaia Compiling and Running

# PLUTO CHARON
# Serial 3:
gcc -Wall -O3 -march=native nbody-s3.c nbody-h.c matrix.c util.c -o nbody-s3 -lm
srun --cpus-per-task=8 ./nbody-s3 1 302400 1000 examples/pluto-charon.npy output.npy

# Parallel 3:
gcc -Wall -O3 -march=native nbody-p3.c nbody-h.c matrix.c util.c -o nbody-p3 -lm -fopenmp
srun --cpus-per-task=8 ./nbody-p3 1 302400 1000 examples/pluto-charon.npy output.npy

# After running, test by plotting
python3 scripts/plot_output.py output.npy