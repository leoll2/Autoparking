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
            Coordinate new_pos = dummy.get_rear_center();
            double new_angle = dummy.get_orientation();
            float dist_from_target;
            switch(ret) {
                case 3:
                case 2:
                    R[s][a] = -200;
                    break;
                case 1:
                    R[s][a] = -100;
                    break;
                case 0:                    
                    dist_from_target = 
                        (new_pos.x - map.target.x) * (new_pos.x - map.target.x) + 
                        (new_pos.y - map.target.y) * (new_pos.y - map.target.y);
                    R[s][a] = (100 / (1 + dist_from_target / 10000)) -
                               8 * abs(new_angle - pi/2);
            }
            if (Vehicle::encode_vehicle(new_angle, new_pos.x, new_pos.y) == target_state)
                R[s][a] = 1000;
        }
    }
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
}
