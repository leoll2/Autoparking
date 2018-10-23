#include <allegro5/allegro.h>	//never remove! (obscure compatibility reasons)
#include <chrono>
#include <csignal>
#include <ctime>
#include <iostream>
#include <memory>
#include <thread>
#include "display.h"
#include "field_params.h"
#include "maps.h"
#include "Q_learning_network.h"
#include "shapes.h"
#include "vehicle.h"

volatile std::sig_atomic_t stopflag = 0;

void halt_simulation(int signal) {
    stopflag = 1;
}

int main(int argc, char **argv) {

    ALLEGRO_DISPLAY *display = NULL;
    std::unique_ptr<Map> map(nullptr);
    
    /* Initialize the random generator */
    srand(time(NULL));

    /* Create the map */
    switch (CHOSEN_MAP) {
        case 1:
            map.reset(new SimpleParkingMap);
            break;
        case 2:
            map.reset(new TwistedParkingMap);
            break;
        default:
            std::cerr << "Non-existing map" << std::endl;
            return -1;
    }

    /* Initialize the Q-learning AI */
    Q_LearningNetwork ai(*map);
    
    /* Initialize the graphics */
    if(!start_graphics(display))
        return -1;
    
    /* Halt the simulation at the end of the current episode in case of SIGINT */
    signal(SIGINT, halt_simulation);
    
    /* Keep simulating episodes of autoparking */
    while(!stopflag) {
        ai.simulate_episode();
    }
    
    /* Close the graphics */
    end_graphics(display);

    std::cout << "The simulation is over!" << std::endl;
    
    return 0;
}
