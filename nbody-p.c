/**
 * Runs a simulation of the n-body problem in 3D.
 * 
 * To compile the program:
 *   gcc -Wall -O3 -fopenmp -march=native nbody-p.c nbody-h.c matrix.c util.c -o nbody-p -lm
 * 
 * To run the program:
 *   ./nbody-s time-step total-time outputs-per-body input.npy output.npy
 * where:
 *   - time-step is the amount of time between steps (Δt, in seconds)
 *   - total-time is the total amount of time to simulate (in seconds)
 *   - outputs-per-body is the number of positions to output per body
 *   - input.npy is the file describing the initial state of the system (below)
 *   - output.npy is the output of the program (see below)
 * 
 * input.npy has a n-by-7 Matrix with one row per body and the columns:
 *   - mass (in kg)
 *   - initial x, y, z position (in m)
 *   - initial x, y, z velocity (in m/s)
 * 
 * output.npy is generated and has a (T/Δt)-by-(3n) Matrix with each row
 * containing the x, y, and z positions of each of the n bodies after a given
 * timestep.
 * 
 * See the PDF for implementation details and other requirements.
 * 
 * AUTHORS: Jack Fineanganofo and Riley Kirkpatrick
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "matrix.h"
#include "nbody-h.h"
#include "util.h"


/**
 * Approximates a single step of the n-body problem. It requires the current
 * step, the number of bodies, the time delta of each step, a Matrix of the
 * positions of bodies, and the a Matrix of bodies.
 */
void approx_nbody(size_t num_steps, size_t n, size_t time_step, Matrix* positions, Body* bodies, size_t num_threads) {
    #pragma omp parallel for num_threads(num_threads) schedule(dynamic, 32)
    for (size_t s = 0; s < num_steps; s++) {
        size_t pos_row = s*positions->cols;
        size_t posi = pos_row;
        for (size_t i = 0; i < n; i++, posi += 3) {
            double mi = bodies[i].mass;
            double xi = positions->data[posi];
            double yi = positions->data[posi + 1];
            double zi = positions->data[posi + 2];

            // Compute acceleration of current body
            double ax = 0, ay = 0, az = 0;
            size_t posj = pos_row;
            for (size_t j = 0; j < n; j++, posj += 3) {
                if (i == j) { continue; }
                double mj = bodies[j].mass;
                double dx = positions->data[posj    ] - xi;
                double dy = positions->data[posj + 1] - yi;
                double dz = positions->data[posj + 2] - zi;
                double r = calc_distance(dx, dy, dz);
                double Fij = calc_grav_force(mi, mj, r);
                // Calculate element of acceleration sum
                ax += Fij*dx/r; ay += Fij*dy/r; az += Fij*dz/r;
            }
            ax /= mi; ay /= mi; az /= mi;
            
            // Compute velocity of current body based on current acceleration
            bodies[i].vx += ax * time_step;
            bodies[i].vy += ay * time_step;
            bodies[i].vz += az * time_step;

            // Compute position of current body for time t+Δt by adding vx*Δt,
            // vy*Δt, and vz*Δt to its positions from time t (respectively)
            positions->data[posi + positions->cols] = xi + bodies[i].vx * time_step;
            positions->data[posi + positions->cols + 1] = yi + bodies[i].vy * time_step;
            positions->data[posi + positions->cols + 2] = zi + bodies[i].vz * time_step;
        }
    }
}

int main(int argc, const char* argv[]) {
    // parse arguments
    if (argc != 6 && argc != 7) { fprintf(stderr, "usage: %s time-step total-time outputs-per-body input.npy output.npy [num-threads]\n", argv[0]); return 1; }
    double time_step = atof(argv[1]), total_time = atof(argv[2]);
    if (time_step <= 0 || total_time <= 0 || time_step > total_time) { fprintf(stderr, "time-step and total-time must be positive with total-time > time-step\n"); return 1; }
    size_t num_outputs = atoi(argv[3]);
    if (num_outputs <= 0) { fprintf(stderr, "outputs-per-body must be positive\n"); return 1; }
    size_t num_threads = argc == 7 ? atoi(argv[6]) : get_num_cores_affinity() / 2; // TODO: you may choose to adjust the default value
    if (num_threads <= 0) { fprintf(stderr, "num-threads must be positive\n"); return 1; }
    Matrix* input = matrix_from_npy_path(argv[4]);
    if (input == NULL) { perror("error reading input"); return 1; }
    if (input->cols != 7) { fprintf(stderr, "input.npy must have 7 columns\n"); return 1; }
    size_t n = input->rows;
    if (n == 0) { fprintf(stderr, "input.npy must have at least 1 row\n"); return 1; }
    if (num_threads > n) { num_threads = n; }
    size_t num_steps = (size_t)(total_time / time_step + 0.5);
    if (num_steps < num_outputs) { num_outputs = 1; }
    size_t output_steps = num_steps/num_outputs;
    num_outputs = (num_steps+output_steps-1)/output_steps;

    // variables available now:
    //   time_step    number of seconds between each time point
    //   total_time   total number of seconds in the simulation
    //   num_steps    number of time steps to simulate (more useful than total_time)
    //   num_outputs  number of times the position will be output for all bodies
    //   output_steps number of steps between each output of the position
    //   num_threads  number of threads to use
    //   input        n-by-7 Matrix of input data
    //   n            number of bodies to simulate

    // start the clock
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create Matrix for storing position data for each step
    Matrix* positions = create_positions_matrix(input, num_steps, n);
    // Create Matrix for storing each body's mass and velocities
    Body* bodies = create_bodies_matrix(input, n);
    
    // Approximate n-body problem for each time step
    approx_nbody(num_steps, n, time_step, positions, bodies, num_threads);

    // Save data into an output Matrix
    Matrix* output = create_output_matrix(num_outputs, output_steps, num_steps, positions);

    // get the end and computation time
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time = get_time_diff(&start, &end);
    printf("%f secs\n", time);

    // save results
    matrix_to_npy_path(argv[5], output);
    // TODO: matrix_to_csv(stdout, output);
    
    // cleanup
    matrix_free(input);
    matrix_free(output);
    matrix_free(positions);
    free(bodies);

    return 0;
}
