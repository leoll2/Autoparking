/* Q-learning neural network parameters */

#ifndef AI_PARAMS_H
#define AI_PARAMS_H

/* Bellman's equation parameters */

#define ALPHA       1   // Learning rate
#define GAMMA       1   // Discount factor
#define EPSILON     0.2   // Exploration factor

/* Convergence detection parameters */

#define CONV_INIT_DELTA     1000
#define CONV_ALPHA          0.0001
#define CONV_THRESHOLD      1

/* Rewards */

#define TARGET_REWARD       1000    // Reach the target
#define OUT_OF_BOX_PUNISH   -200    // Exit from the range of acceptable states (angle, position)
#define COLLISION_PUNISH    -100    // Collide with one obstacle
#define MOVEMENT_PUNISH     -5      // Perform a legal maneuver
#endif	// AI_PARAMS_H

