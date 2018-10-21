/* Q-learning neural network parameters */

#ifndef AI_PARAMS_H
#define AI_PARAMS_H

/* Bellman's equation parameters */

#define ALPHA       1.0     // Learning rate
#define GAMMA       1.0     // Discount factor
#define EPSILON     0.2     // Exploration factor

/* Stop criteria */

#define STOP_MAX_ITER       1       // Enable stopping after maximum number of iterations
#define STOP_CONVERGENCE    1       // Enable stopping after reaching convergence
#define MAX_ITER_FACTOR     10      // Stop after (factor * n_states * n_actions) iterations

/* Convergence estimation metric parameters */

#define CONV_INIT_DELTA     1000
#define CONV_ZETA           0.0001
#define CONV_THRESHOLD      1.0

/* Rewards */

#define TARGET_REWARD       1000    // Reach the target
#define OUT_OF_BOX_PUNISH   -200    // Exit from the range of acceptable states (angle, position)
#define COLLISION_PUNISH    -100    // Collide with one obstacle
#define MOVEMENT_PUNISH     -5      // Perform a legal maneuver

/* Stats */

#define LOG_CONV_METRIC     1       // Enable logging the evolution of convergence metric
#define LOG_ITER_INTERVAL   50000   // One line of log every LOG_ITER_INTERVAL iterations

#endif	// AI_PARAMS_H

