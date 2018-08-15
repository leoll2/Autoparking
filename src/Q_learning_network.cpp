#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <thread>
#include "display.h"
#include "Q_learning_network.h"
#include "vehicle_params.h"
#include "vehicle.h"

bool Q_LearningNetwork::initialize_Q() {
    if (restore_from_cache())
        return true;
    else
        return false;
}

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
            
            // Retrieve the maneuver corresponding to this action
            Maneuver mvn(a);
            
            // Compute the next state
            unsigned int ret = dummy.move(map, mvn);
            
            // Compute the reward function for the current state on the basis of the next one
            /*Coordinate new_pos = dummy.get_rear_center();
            double new_angle = dummy.get_orientation();
            float dist_from_target;*/
            switch(ret) {
                case 3:
                case 2:
                    R[s][a] = OUT_OF_BOX_PUNISH;
                    break;
                case 1:
                    R[s][a] = COLLISION_PUNISH;
                    break;
                case 0:
                    /*dist_from_target = 
                        (new_pos.x - map.target.x) * (new_pos.x - map.target.x) + 
                        (new_pos.y - map.target.y) * (new_pos.y - map.target.y);*/
                    unsigned int s2 = dummy.encode();
                    if (s2 == target_state) {
                        R[s][a] = TARGET_REWARD;
                    } else
                        R[s][a] = MOVEMENT_PUNISH;
                        // R[s][a] = (100 / (1 + dist_from_target / 10000)) - 8 * abs(new_angle - pi/2);
            }
        }
    }
}

void Q_LearningNetwork::train(unsigned int iterations) {
    
    unsigned int iter = 0;
    do {
        ++iter;
        if (iter % 50000 == 0)
            std::cout << "Iterations: " << iter << std::endl;
        Vehicle dummy = Vehicle::random_vehicle();
        unsigned int s1 = dummy.encode();
        if (s1 == target_state) // if the vehicle spawned in the final state, abort this episode
            continue;

        unsigned int ret;
        do {
            s1 = dummy.encode();
            Maneuver mnv = Maneuver::random_maneuver();
            unsigned int a = mnv.encode_maneuver();
            ret = dummy.move(map, mnv);
            if (ret) {
                Q[s1][a] = Q[s1][a] * (1 - ALPHA) + ALPHA * (R[s1][a] + GAMMA * get_max_state_quality(s1));
            } else {
                //float old_Q_value = Q[s1][a];       //DEBUG CONVERGENCE
                unsigned int s2 = dummy.encode();
                Q[s1][a] = Q[s1][a] * (1 - ALPHA) + ALPHA * (R[s1][a] + GAMMA * get_max_state_quality(s2));
                if (s2 == target_state) // stop the episode if the car reached the final state
                    break;
                /*if (iterations % 1000 == 0)   //DEBUG CONVERGENCE
                    std::cout << "newQ: " << Q[s1][a] << " deltaQ: " << (Q[s1][a] - old_Q_value) << std::endl;*/
                assert (Q[s1][a] <= TARGET_REWARD && "Q-matrix is diverging!");
            }
        } while (ret == 0);
    } while (iter < iterations);
    std::cout << "Finished training for " << iterations << " iterations!" << std::endl;
    store_into_cache();
}

bool Q_LearningNetwork::restore_from_cache() {
    std::ifstream r("cache/R");
    std::ifstream q("cache/Q");
    
    if (r.is_open() && q.is_open()) {
        unsigned int s_r, a_r, s_q, a_q;
        r >> s_r >> a_r;
        q >> s_q >> a_q;
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

bool Q_LearningNetwork::store_into_cache() {
    std::ofstream r("cache/R");
    std::ofstream q("cache/Q");
    if (r.is_open() && q.is_open()) {
        r << n_states << " " << n_actions << "\n";
        q << n_states << " " << n_actions << "\n";
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

unsigned int Q_LearningNetwork::get_best_action(int s) {
    return std::distance(Q[s].begin(), std::max_element(Q[s].begin(), Q[s].end()));
}

float Q_LearningNetwork::get_max_state_quality(int s) {
    return *std::max_element(Q[s].begin(), Q[s].end());
}

Q_LearningNetwork::Q_LearningNetwork(Map& m) :
    n_states(HREF_POINTS * VREF_POINTS * ANGLE_REFS),
    n_actions(30),
    Q(n_states, std::vector<float>(n_actions, 0)),
    R(n_states, std::vector<float>(n_actions, 0)),
    map(m),
    target_state(Vehicle::encode_vehicle(pi/2, map.target.x, map.target.y))
{
    std::cout << "Initializing AI..." << std::endl;
    initialize_R();
    std::cout << "Matrix R ready" << std::endl;
    if (!initialize_Q()) {
        unsigned int n_iterations = 10 * n_states * n_actions;
        train(n_iterations);
    }
    std::cout << "Matrix Q ready" << std::endl;
}

unsigned int Q_LearningNetwork::get_n_states() const {
    return n_states;
}

unsigned int Q_LearningNetwork::get_n_actions() const {
    return n_actions;
}

Map& Q_LearningNetwork::get_map() const {
    return map;
}

double Q_LearningNetwork::get_quality(unsigned int s, unsigned int a) const {
    return Q[s][a];
}

double Q_LearningNetwork::get_reward(unsigned int s, unsigned int a) const {
    return R[s][a];
}

void Q_LearningNetwork::simulate_episode() {
    // Spawn the vehicle in a random legal position
    Vehicle car = Vehicle::random_vehicle();
    while (!map.is_within_boundaries(car.to_polygon()) || 
            map.collides_with_obstacles(car.to_polygon()))
        car = Vehicle::random_vehicle();
    
    // Show it
    display_all_entities(map, car); 
    
    unsigned int ret_move;                          // return value of car.move()
    std::set<unsigned int> visited;                 // keep track of visited states to abort loops
    std::pair<std::set<unsigned int>::iterator,bool> ret_visit;  // return value of visited.insert()
    unsigned int state = car.encode();
    
    // Start performing maneuvers
    do {
        // Mark the current state as visited
        ret_visit = visited.insert(state);
        // Check whether the state had already been visited
        if (ret_visit.second == false)
            return;
        // Clone the vehicle in the current position
        Vehicle ghost(car);
        // Choose the (hopefully) best maneuver
        Maneuver mnv(get_best_action(state));
        // Move the vehicle accordingly
        ret_move = car.move(map, mnv);
        // Show the new position
        //display_all_entities(map, car);
        display_all_entities_enhanced(map, ghost, car, 10);
        // If the vehicle reached the final state, wait a bit then return
        if ((ret_move == 0) && ((state = car.encode()) == target_state)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
            return;
        } else {    // else just wait a little
            //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    } while (ret_move == 0);
}