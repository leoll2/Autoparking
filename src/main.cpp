#include <allegro5/allegro.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <set>
#include <thread>
#include "display.h"
#include "field_params.h"
#include "keyboard.h"
#include "maps.h"
#include "Q_learning_network.h"
#include "shapes.h"
#include "vehicle.h"


std::vector<std::unique_ptr<Map>> maps;


void simulate(Q_LearningNetwork& ai) {

    Map *map = ai.get_map();
    // Spawn the vehicle in a random legal position
    Vehicle car = Vehicle::random_vehicle();
    while (!map->is_within_boundaries(car.to_polygon()) || map->collides_with_obstacles(car.to_polygon()))
        car = Vehicle::random_vehicle();

    // Keep generating new episodes
    while(true) {
        unsigned int n_moves = 0;
        unsigned int state;
        unsigned int ret_move;                  // return value of car.move()
        std::set<unsigned int> visited;         // keep track of visited states to abort loops
        std::pair<std::set<unsigned int>::iterator,bool> ret_visit;  // return value of visited.insert()

        if (should_stop)
            return;
        if (should_change_map) {
            current_map = (current_map + 1) % 2;
            ai.set_map(*maps[current_map]);
            map = ai.get_map();
            should_change_map = false;
        }
        if (should_reset) {
            ai.reset_Q();
            should_reset = false;
        }
        if (should_store_cache) {
            ai.store_into_cache();
            should_store_cache = false;
        }
        if (should_load_cache) {
            ai.restore_from_cache();
            should_load_cache = false;
        }
        while (should_train) {
            ai.train(ai.get_n_states() * ai.get_n_actions() / 10, true);
            display_all_entities(*map, ai, car);
            poll_commands();
            if (should_stop)
                return;
        }
        
        car = Vehicle::random_vehicle();
        while (!map->is_within_boundaries(car.to_polygon()) || map->collides_with_obstacles(car.to_polygon()))
            car = Vehicle::random_vehicle();
        state = car.encode();

        // Show the initial state of the episode
        display_all_entities(*map, ai, car);

        // Keep performing maneuvers
        do {
            // Handle possible quit, reset or pause request
            poll_commands();
            if (should_stop || should_reset || should_train || should_change_map)
                break;
            if (should_store_cache) {
                ai.store_into_cache();
                should_store_cache = false;
            }
            if (should_load_cache) {
                ai.restore_from_cache();
                should_load_cache = false;
            }
            while(paused) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                poll_commands();
                if (should_stop)
                    break;
            }

            // Mark the current state as visited
            ret_visit = visited.insert(state);
            
            // Check whether the state had already been visited
            if (ret_visit.second == false)
                break;
             
            // Clone the vehicle in the current position
            Vehicle ghost(car);
            
            // Choose the (hopefully) best maneuver
            Maneuver mnv(ai.get_best_action(state));
            
            // Move the vehicle accordingly
            ret_move = car.move(*map, mnv);
            ++n_moves;
            
            // Show the new position
            display_all_entities_enhanced(*map, ai, ghost, car, 10, 50);
            
            // If the vehicle reached the final state, wait a bit then return
            if ((ret_move == 0) && ((state = car.encode()) == ai.get_target_state())) {
                //std::cout << n_moves << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                break;
            }
        } while (ret_move == 0);
    }
}



int main(int argc, char **argv) {

    ALLEGRO_DISPLAY *display = NULL;
    
    /* Initialize the random generator */
    srand(time(NULL));

    /* Create the map */
    std::unique_ptr<Map> map0(new SimpleParkingMap);
    std::unique_ptr<Map> map1(new TwistedParkingMap);

    maps.push_back(std::move(map0));
    maps.push_back(std::move(map1));

    /* Initialize the Q-learning AI */
    Q_LearningNetwork ai(*maps[current_map]);
    
    /* Initialize the graphics */
    if(!start_graphics(display))
        return -1;

    /* Initialize the keyboard */
    if(!start_keyboard())
        return -1;

    /* Simulate episodes of autoparking*/
    simulate(ai);
    
    /* Release the keyboard */
    stop_keyboard();

    /* Close the graphics */
    end_graphics(display);

    std::cout << "The simulation is over!" << std::endl;
    
    return 0;
}
