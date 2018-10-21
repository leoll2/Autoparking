#include <allegro5/allegro.h>	//never remove! (obscure compatibility reasons)
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include "display.h"
#include "field_params.h"
#include "maps.h"
#include "Q_learning_network.h"
#include "shapes.h"
#include "vehicle.h"

int main(int argc, char **argv) {

    srand(time(NULL));

    ALLEGRO_DISPLAY *display = NULL;

    /* Create the map */
    SimpleParkingMap map;

    Q_LearningNetwork ai(map);
    
    if(!start_graphics(display))
        return -1;
    
    while(1) {
        ai.simulate_episode();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    end_graphics(display);

    return 0;
}