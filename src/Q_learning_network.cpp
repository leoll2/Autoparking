#include <algorithm>
#include <iostream>
#include "Q_learning_network.h"
#include "vehicle_params.h"
#include "vehicle.h"

void Q_LearningNetwork::initialize_Q() {
    return;     // leave Q to default zero
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
                    R[s][a] = -200;
                    break;
                case 1:
                    R[s][a] = -100;
                    break;
                case 0:
                    /*dist_from_target = 
                        (new_pos.x - map.target.x) * (new_pos.x - map.target.x) + 
                        (new_pos.y - map.target.y) * (new_pos.y - map.target.y);*/
                    unsigned int s2 = dummy.encode();
                    if (s2 == target_state) {
                        R[s][a] = 1000;
                        std::cout << "R[" << s << "][" << a << "] -> s2: " << s2 << "with reward 1000" << std::endl; 
                    } else
                        R[s][a] = -5;
                        // R[s][a] = (100 / (1 + dist_from_target / 10000)) - 8 * abs(new_angle - pi/2);
            }
        }
    }
}

void Q_LearningNetwork::train() {
    std::cout<< "target state: " << target_state << std::endl;
    unsigned int iterations = 0;
    Vehicle dummy = Vehicle::random_vehicle();
    do {
        ++iterations;
        if (iterations % 50000 == 0)
            std::cout << "Iterations: " << iterations << std::endl;
        Vehicle dummy = Vehicle::random_vehicle();
        unsigned int s1 = Vehicle::encode_vehicle(dummy.get_orientation(), dummy.get_rear_center().x, dummy.get_rear_center().y);
        if (s1 == target_state) // if the vehicle spawned in the final state, abort this episode
            continue;
        //std::cout << "S1| angle: " << dummy.get_orientation() << " x: " << dummy.get_rear_center().x << " y: " << dummy.get_rear_center().y << std::endl;
        unsigned int ret;
        do {
            Maneuver mnv = Maneuver::random_maneuver();
            unsigned int a = mnv.encode_maneuver();
            ret = dummy.move(map, mnv);
            if (ret) {
                Q[s1][a] = Q[s1][a] * (1 - ALPHA) + ALPHA * (R[s1][a] + GAMMA * get_max_state_quality(s1));
            } else {
                float old_Q_value = Q[s1][a];       //DEBUG
                unsigned int s2 = dummy.encode();
                Q[s1][a] = Q[s1][a] * (1 - ALPHA) + ALPHA * (R[s1][a] + GAMMA * get_max_state_quality(s2));
                if (s2 == target_state) // stop the episode if the car reached the final state
                    break;
                if (iterations % 1000 == 0)
                    std::cout << "newQ: " << Q[s1][a] << " deltaQ: " << (Q[s1][a] - old_Q_value) << std::endl;
                if (Q[s1][a] > 1000) {
                    std::cout << "s1: " << s1 << " a: " << a << " oldQvalue: " << old_Q_value
                             << " s2: " << s2 << " max(s2): " << get_max_state_quality(s2) << " r: " << R[s1][a] << std::endl;
                    exit(20);
                }
            }
        } while (ret == 0);
    } while (iterations < 10 * n_states * n_actions);
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
    std::cout << "Starting initialization of AI..." << std::endl;
    initialize_Q();
    std::cout << "Matrix Q ready" << std::endl;
    initialize_R();
    std::cout << "Matrix R ready" << std::endl;
    train();
    std::cout << "Trained!" << std::endl;
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