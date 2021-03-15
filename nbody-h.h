/**
 * Helper functions used by all nbody solutions.
 * 
 * AUTHORS: Jack Fineanganofo and Riley Kirkpatrick
 */

/**
 * A body structure that contains its mass and velocity.
 */
typedef struct {
    double mass;
    double vx, vy, vz;
} Body;


/**
 * Calculates Euclidean distance
 * r_ij = sqrt(∆x^2 + ∆y^2 + ∆z^2) + 𝜀
 */
double calc_distance(double dx, double dy, double dz);

/**
 * Calculates the gravitational force between two objects
 * F_ij = G(m_i * m_j) / r_ij^2
 */
double calc_grav_force(double mi, double mj, double r);

/**
 * Create Matrix for body positions of each step of nbody approximation.
 * Moves the initial positions into the Matrix's first row.
 */
Matrix* create_positions_matrix(Matrix* input, size_t num_steps, size_t n);

/**
 * Create Body array for storing each body's mass and velocity.
 */
Body* create_bodies_matrix(Matrix* input, size_t n);

/**
 * Takes all positions from an n-body approximation that has num_steps and
 * creates an output matrix with only num_outputs by stepping by output_steps
 * in the positions Matrix.
 */
Matrix* create_output_matrix(size_t num_outputs, size_t output_steps, size_t num_steps, Matrix* positions);