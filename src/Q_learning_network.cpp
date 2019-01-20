#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <thread>
#include "display.h"
#include "field_params.h"
#include "Q_learning_network.h"
#include "vehicle_params.h"
#include "vehicle.h"


/******************************************************************************/
/*     Q-LEARNING NETWORK: reinforcement learning applied to autoparking      */
/******************************************************************************/

/**
 * Initialize quality matrix Q.
 * 
 * @return true if loaded from cache, false otherwise
 */
bool Q_LearningNetwork::initialize_Q() {
    if (restore_from_cache())
        return true;
    else
        return false;
}


/**
 * Initialize rewards matrix R.
 */
void Q_LearningNetwork::initialize_R() {
    
    // Initialize a dummy car to be used for rewards computation
    Vehicle dummy(CAR_LENGTH, CAR_WIDTH, Coordinate(0,0), 0);
    
    // For each state-action pair...
    for (unsigned int s = 0; s < n_states; ++s) {
        for (unsigned int a = 0; a < n_actions; ++a) {
            
            // Get the position corresponding to this state
            std::vector<double> car_state = Vehicle::decode_vehicle(s);
            double initial_angle = car_state[0];
            Coordinate initial_pos(car_state[1], car_state[2]);
            assert(Vehicle::encode_vehicle(car_state[0], car_state[1], car_state[2]) == s && "Encode-decode buggy");
            
            // Position the dummy vehicle here
            dummy.reposition(map, initial_pos, initial_angle);
            
            // Retrieve the maneuver object corresponding to this action code
            Maneuver mvn(a);
            
            // Compute the next state
            unsigned int ret = dummy.move(map, mvn);
            
            // Compute the reward function for the current state on the basis of the next one
            switch(ret) {
                case 3:
                case 2:
                    R[s][a] = OUT_OF_BOX_PUNISH;
                    break;
                case 1:
                    R[s][a] = COLLISION_PUNISH;
                    break;
                case 0:
                    unsigned int s2 = dummy.encode();
                    if (s2 == target_state) {
                        R[s][a] = TARGET_REWARD;
                    } else
                        R[s][a] = MOVEMENT_PUNISH;
            }
        }
    }
}


/**
 * Get the best possible action (exploiting current knowledge) for a given state.
 * 
 * @param s the given state
 * @return the code of the action
 */
unsigned int Q_LearningNetwork::get_best_action(int s) const {

    unsigned int a = std::distance(Q[s].begin(), std::max_element(Q[s].begin(), Q[s].end()));
    // If there is no maximum (i.e. uninitialized Q), returns a random one
    if (Q[s][a] == 0)
        a = rand() % this->n_actions;

    return a;
}


/**
 * Get an action according to the epsilon-greedy policy.
 * 
 * @param s the given state
 * @return the code of the action
 */
unsigned int Q_LearningNetwork::get_eps_greedy_action(int s) const {
    
    // Generate random number between 0 and 1
    double r = (double) rand() / (RAND_MAX);
    
    /* If the random number is less than epsilon, return a random action
    *  else return the best action, according to policy */
    if (r < EPSILON)
        return rand() % this->n_actions;
    else
        return this->get_best_action(s);
}


/**
 * Get the maximum value of Q of a given state for all the possible actions.
 * 
 * @param s the given state
 * @return the value of Q for that state-action pair
 */
float Q_LearningNetwork::get_max_state_quality(int s) const {
    return *std::max_element(Q[s].begin(), Q[s].end());
}


/**
 * Train the neural network for at most a specified number of iterations.
 * 
 * @param iterations the max number of training iterations to be performed
 */
void Q_LearningNetwork::train(unsigned int iterations, bool force) {
    
    std::ofstream log;
    unsigned int iter = 0;
    float avg_delta = CONV_INIT_DELTA;
    
    // Start logging the convergence estimation metric values (if option selected)
    if (LOG_CONV_METRIC)
        this->open_conv_log(log);
    
    // Iterate until max number of iterations reached or acceptable convergence achieved
    do {
        unsigned int ret;
        
        // Log, display and update info
        if (LOG_CONV_METRIC && (iter % LOG_ITER_INTERVAL == 0)) {
            this->add_to_conv_log(log, iter_trained, avg_delta);
            //std::cout << "Iterations: " << iter << " \tdelta: " << avg_delta << std::endl;    //TODO: stampare a schermo, avg_delta deve diventare attributo
        }
        ++iter;
        ++iter_trained;
        
        // Spawn the vehicle in a random position
        Vehicle dummy = Vehicle::random_vehicle();
        unsigned int s1 = dummy.encode();
        
        // If the vehicle spawned in the final state, abort this episode
        if (s1 == target_state)
            continue;

        // Keep moving the car as long as it doesn't crash or exit map
        do {
            // Get current state and action to be performed
            s1 = dummy.encode();
            unsigned int a = get_eps_greedy_action(s1);
            Maneuver mnv(a);
            
            // Move the vehicle
            ret = dummy.move(map, mnv);
            
            // Update Q and estimate the convergence
            float old_Q_value = Q[s1][a];
            if (ret) {
                Q[s1][a] = old_Q_value * (1 - ALPHA) + ALPHA * (R[s1][a] + GAMMA * get_max_state_quality(s1));
            } else {
                unsigned int s2 = dummy.encode();
                Q[s1][a] = old_Q_value * (1 - ALPHA) + ALPHA * (R[s1][a] + GAMMA * get_max_state_quality(s2));
                if (s2 == target_state) // stop the episode if the car reached the final state
                    break;
                assert (Q[s1][a] <= TARGET_REWARD && "Q-matrix is diverging!");
                avg_delta = (1-CONV_ZETA) * avg_delta + CONV_ZETA * std::abs(Q[s1][a] - old_Q_value);
            }
        } while (ret == 0);
    } while ((iter < iterations || !STOP_MAX_ITER) && (force || avg_delta > CONV_THRESHOLD || !STOP_CONVERGENCE));
    
    // Tell the user which stop criterion was triggered, and number of iterations
    /*if (iter >= iterations)
        std::cout << "Training stopped after reaching the maximum limit of iterations." << std::endl;
    else
        std::cout << "Training stopped due to convergence." << std::endl;
    std::cout << "Performed iterations: " << iter << std::endl;*/

    // Close the log
    if (LOG_CONV_METRIC)
        this->close_conv_log(log);
}


/**
 * Restore from the cache the values of matrices Q and R.
 * 
 * @return true if success, false otherwise
 */
bool Q_LearningNetwork::restore_from_cache() {
    
    /*std::ifstream r("cache/R");
    std::ifstream q("cache/Q");*/
    
    std::ifstream r, q;
    
    switch (CHOSEN_MAP) {
        case 1:
            r.open("cache/R_1", std::ifstream::in);
            q.open("cache/Q_1", std::ifstream::in);
            break;
        case 2:
            r.open("cache/R_2", std::ifstream::in);
            q.open("cache/Q_2", std::ifstream::in);
            break;
        default:
            return false;
    }
    
    if (r.is_open() && q.is_open()) {
        unsigned int s_r, a_r, s_q, a_q;
        r >> s_r >> a_r;
        q >> s_q >> a_q >> iter_trained;
        if (s_r != n_states || a_r != n_actions || s_q != n_states || a_q != n_actions) {
            std::cerr << "Loading from cache failed: dimension mismatch." << std::endl;
            return false;
        }
        for (unsigned int s = 0; s < n_states; ++s) {
            for (unsigned int a = 0; a < n_actions; ++a) {
                r >> this->R[s][a];
                q >> this->Q[s][a];
            }
        }
    } else {
        std::cerr << "Loading from cache failed: couldn't open file." << std::endl;
        return false;
    }
    r.close();
    q.close();
    if (!r || !q) {
        std::cerr << "Loading from cache failed: error while reading." << std::endl;
        return false;
    }
    std::cout << "Cache restored" << std::endl;
    return true;
}


/**
 * Store into cache the values of matrices Q and R.
 * 
 * @return true if success, false otherwise
 */
bool Q_LearningNetwork::store_into_cache() {
    
    /*std::ofstream r("cache/R");
    std::ofstream q("cache/Q");*/
    std::ofstream r, q;
    
    switch (CHOSEN_MAP) {
        case 1:
            r.open("cache/R_1", std::ofstream::out);
            q.open("cache/Q_1", std::ofstream::out);
            break;
        case 2:
            r.open("cache/R_2", std::ofstream::out);
            q.open("cache/Q_2", std::ofstream::out);
            break;
        default:
            return false;
    }
    
    if (r.is_open() && q.is_open()) {
        r << n_states << " " << n_actions << "\n";
        q << n_states << " " << n_actions << " " << iter_trained << "\n";
        for (unsigned int s = 0; s < n_states; ++s) {
            for (unsigned int a = 0; a < n_actions; ++a) {
                r << this->R[s][a] << " ";
                q << this->Q[s][a] << " ";
            }
        }
    } else {
        std::cerr << "Updating cache failed: couldn't open file." << std::endl;
        return false;
    }
    r.close();
    q.close();
    if (!r || !q) {
        std::cerr << "Updating cache failed: error while reading." << std::endl;
        return false;
    }
    std::cout << "Cache updated" << std::endl;
    return true;
}


/**
 * Open the file for convergence logging.
 * 
 * @param log the log ofstream object
 */
void Q_LearningNetwork::open_conv_log(std::ofstream& log) {
    char filename[32];
    snprintf(filename, 32, "stats/%3.2lf_%3.2lf_%3.2lf.txt", ALPHA, GAMMA, EPSILON);
    log.open(filename, std::ofstream::out);
    if (log.is_open()) {
        log << "alpha: " << ALPHA << " gamma: " << GAMMA << " epsilon: " << EPSILON << std::endl;
    }
}


/**
 * Close the file for convergence logging.
 * 
 * @param log the log ofstream object
 */
void Q_LearningNetwork::close_conv_log(std::ofstream& log) {
    log.close();
}


/**
 * Add a new line of logging.
 * 
 * @param log the log ofstream object
 * @param iter iterations (to be logged)
 * @param avg_delta convergence estimation (to be logged)
 */
void Q_LearningNetwork::add_to_conv_log(std::ofstream& log, unsigned int iter, float avg_delta) {
    if (log.is_open()) {
        log << iter << " " << avg_delta << std::endl;
    }
}


/**
 * Constructor for a Q-learning neural network for autonomous pparking.
 * 
 * @param m the map used for simulations
 */
Q_LearningNetwork::Q_LearningNetwork(Map& m) :
    n_states(HREF_POINTS * VREF_POINTS * ANGLE_REFS),
    n_actions(30),
    Q(n_states, std::vector<float>(n_actions, 0)),
    R(n_states, std::vector<float>(n_actions, 0)),
    map(m),
    iter_trained(0),
    target_state(Vehicle::encode_vehicle(pi/2, map.target.x, map.target.y))
{
    // Print initialization information
    std::cout << "Initializing AI..." << std::endl
            << "Alpha: " << ALPHA << std::endl
            << "Gamma: " << GAMMA << std::endl
            << "Epsilon: " << EPSILON << std::endl
            << "Limit number of iterations: " << (STOP_MAX_ITER ?
                    "enabled" : "disabled") << std::endl
            << "Max number of iterations: " << (STOP_MAX_ITER ? 
                    std::to_string(MAX_ITER_FACTOR * n_states * n_actions) : "-") << std::endl
            << "Detect convergence: " << (STOP_CONVERGENCE ? 
                    "enabled" : "disabled") << std::endl
            << "Convergence initial delta: " << (STOP_CONVERGENCE ?
                    std::to_string(CONV_INIT_DELTA) : "-") << std::endl
            << "Convergence zeta: " << (STOP_CONVERGENCE ? 
                    std::to_string(CONV_ZETA) : "-") << std::endl
            << "Convergence threshold: " << (STOP_CONVERGENCE ? 
                    std::to_string(CONV_THRESHOLD) : "-") << std::endl; 
    
    // Initialize rewards matrix R
    initialize_R();
    std::cout << "Matrix R ready" << std::endl;
    
    // Initialize quality matrix Q
    if (initialize_Q())
        std::cout << "Q initialized with cached values" << std::endl;
    else
        std::cout << "Q left uninitialized (cache not found)" << std::endl;
    /*if (!initialize_Q()) {
        unsigned int n_iterations = static_cast<unsigned int>(MAX_ITER_FACTOR * n_states * n_actions);
        train(n_iterations);
    }*/
    //std::cout << "Matrix Q ready" << std::endl;
}


/**
 * Get the number of states of the networks.
 * 
 * @return number of states
 */
unsigned int Q_LearningNetwork::get_n_states() const {
    return n_states;
}


/**
 * Get the number of possible actions.
 * 
 * @return number of actions
 */
unsigned int Q_LearningNetwork::get_n_actions() const {
    return n_actions;
}


/**
 * Get the map used for simulation.
 * 
 * @return reference to map object
 */
Map& Q_LearningNetwork::get_map() const {
    return map;
}


/**
 * Get the quality value of a state-action pair.
 * 
 * @param s the state
 * @param a the action
 * @return quality value
 */
double Q_LearningNetwork::get_quality(unsigned int s, unsigned int a) const {
    return Q[s][a];
}


/**
 * Get the reward value associated to a state-action pair
 * 
 * @param s the state
 * @param a the action
 * @return reward value
 */
double Q_LearningNetwork::get_reward(unsigned int s, unsigned int a) const {
    return R[s][a];
}


/**
* Get the code associated to the target state.
*/
unsigned int Q_LearningNetwork::get_target_state() const {
    return target_state;
}


/**
* Get the number of training iterations performed.
*/
unsigned int Q_LearningNetwork::get_iter_trained() const {
    return iter_trained;
}


/**
* Reset the Q matrix to zero values.
*/
void Q_LearningNetwork::reset_Q() {

    for (auto &s : Q)
        std::fill(s.begin(), s.end(), 0.0);

    iter_trained = 0;
}