/**
 * Helper functions used by all nbody solutions.
 * 
 * AUTHORS: Jack Fineanganofo and Riley Kirkpatrick
 */

#include <math.h>
#include <string.h>

#include "matrix.h"
#include "nbody-h.h"


// Gravitational Constant in N m^2 / kg^2 or m^3 / kg / s^2
#define G 6.6743015e-11
// Softening factor to reduce divide-by-near-zero effects
#define SOFTENING 1e-9


/**
 * Calculates Euclidean distance
 * r_ij = sqrt(∆x^2 + ∆y^2 + ∆z^2) + 𝜀
 */
double calc_distance(double dx, double dy, double dz) {
    return sqrt(dx*dx + dy*dy + dz*dz) + SOFTENING;
}

/**
 * Calculates the gravitational force between two objects
 * F_ij = G(m_i * m_j) / r_ij^2
 */
double calc_grav_force(double mi, double mj, double r) {
    return G*mi*mj / (r*r);
}

/**
 * Create Matrix for body positions of each step of nbody approximation.
 * Moves the initial positions into the Matrix's first row.
 */
Matrix* create_positions_matrix(Matrix* input, size_t num_steps, size_t n) {
    // Add one row for initial positions
    Matrix* positions = matrix_zeros(num_steps + 1, 3*n);
    // Add one to skip over mass of each body
    double* restrict init_pos_data = input->data + 1;
    double* restrict pos_data = positions->data;
    for (size_t i = 0; i < n; i++, init_pos_data += input->cols, pos_data += 3) {
        // Move the initial position of a body into the positions Matrix
        memcpy(pos_data, init_pos_data, 3*sizeof(double));
    }
    return positions;
}

/**
 * Create Body array for storing each body's mass and velocity.
 */
Body* create_bodies_matrix(Matrix* input, size_t n) {
    Body* bodies = (Body*)malloc(n*sizeof(Body));
    double* restrict input_data = input->data;
    for (size_t i = 0; i < n; i++, input_data += input->cols) {
        // Mass of the body
        bodies[i].mass = input_data[0];
        // Initial velocity of the body
        bodies[i].vx = input_data[4];
        bodies[i].vy = input_data[5];
        bodies[i].vz = input_data[6];
    }
    return bodies;
}

/**
 * Takes all positions from an n-body approximation that has num_steps and
 * creates an output matrix with only num_outputs by stepping by output_steps
 * in the positions Matrix.
 */
Matrix* create_output_matrix(size_t num_outputs, size_t output_steps, size_t num_steps, Matrix* positions) {
    Matrix* output = matrix_zeros(num_outputs, positions->cols);
    // Save first row of positions in the output
    memcpy(output->data, positions->data, positions->cols*sizeof(double));
    // Get pointer to output data at 2nd row
    double* restrict output_data = output->data + output->cols;
    size_t rows_to_skip = output_steps*positions->cols;
    // Get pointer to position at the (output_steps + 1)th row since we need to
    // pass the initial position and increment by output_steps
    double* restrict pos_data = positions->data + rows_to_skip + positions->cols;
    // Loop 2 less than output_steps times since we add first and last positions
    for (size_t i = 0; i < num_steps - 2*output_steps; i += output_steps) {
        // Save num_outputs rows of data into the output Matrix
        memcpy(output_data, pos_data, positions->cols*sizeof(double));
        pos_data += rows_to_skip;
        output_data += output->cols;
    }
    // Save last row of positions in the output
    memcpy(output->data + positions->cols*(num_outputs-1), positions->data + positions->cols*(num_steps), positions->cols*sizeof(double));
    return output;
}
