#include <allegro5/allegro.h>	//never remove! (obscure compatibility reasons)
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include "display.h"
#include "field_params.h"
#include "map.h"
#include "Q_learning_network.h"
#include "shapes.h"
#include "vehicle.h"

int main(int argc, char **argv) {

    srand(time(NULL));

    ALLEGRO_DISPLAY *display = NULL;

    if(!start_graphics(display))
            return -1;

    /* Create the map */
    Map map(HREF_POINTS, VREF_POINTS, Coordinate(8*SPACE_UNIT, 8*SPACE_UNIT));

    /* Add two rectangular obstacles */
    std::vector<Coordinate> ob1_coords = {	//must be counterclockwise
            {7*SPACE_UNIT, 	18*SPACE_UNIT},
            {12*SPACE_UNIT, 18*SPACE_UNIT},
            {12*SPACE_UNIT, 24*SPACE_UNIT},
            {7*SPACE_UNIT, 	24*SPACE_UNIT}};
    Polygon ob1(ob1_coords);
    map.add_obstacle(ob1);

    std::vector<Coordinate> ob2_coords = {	//must be counterclockwise
            {7*SPACE_UNIT, 	0*SPACE_UNIT},
            {12*SPACE_UNIT, 0*SPACE_UNIT},
            {12*SPACE_UNIT, 6*SPACE_UNIT},
            {7*SPACE_UNIT, 	6*SPACE_UNIT}};
    Polygon ob2(ob2_coords);
    map.add_obstacle(ob2);

    Q_LearningNetwork ai(map);

    /*display_rewards(ai);
    std::cout << "Rewards are now displayed" << std::endl;
     while(1);*/
    
    while(1) {
        ai.simulate_episode();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    end_graphics(display);

    return 0;
}