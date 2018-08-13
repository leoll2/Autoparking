#include <allegro5/allegro.h>	//never remove! (obscure compatibility reasons)
#include <chrono>
#include <iostream>
#include <thread>
#include "display.h"
#include "field_params.h"
#include "map.h"
#include "shapes.h"
#include "vehicle.h"
#include "vehicle_params.h"

int main(int argc, char **argv) {
	
	ALLEGRO_DISPLAY *display = NULL;
	
	if(!start_graphics(display))
		return -1;

	/* Create the map */
	Map map(HREF_POINTS, VREF_POINTS, Coordinate(0,0));
	
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

	/* Keep spawning the car in random positions */
	while(1) {
		//Vehicle car = Vehicle::random_vehicle();
		Vehicle car = Vehicle(CAR_LENGTH, CAR_WIDTH, Coordinate(4*SPACE_UNIT, 8*SPACE_UNIT), pi/2);
		display_all(map, car);
		unsigned int ret;
		do {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			Maneuver mnv = Maneuver::random_maneuver();
			//Maneuver mnv = Maneuver(5);
                        std::cout << "Maneuver code: \t" << mnv.encode_maneuver() << std::endl;
			ret = car.move(map, mnv);
			display_all(map, car);
		} while (ret == 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	end_graphics(display);

	return 0;
}