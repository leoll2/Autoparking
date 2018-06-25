#include <iostream>
#include <allegro5/allegro.h>	//never remove! (obscure compatibility reasons)
#include "display.h"
#include "field_params.h"
#include "map.h"
#include "shapes.h"
#include "vehicle.h"

int main(int argc, char **argv) {
	
	ALLEGRO_DISPLAY *display = NULL;
	
	if(!start_graphics(display))
		return -1;

	Map m(HREF_POINTS, VREF_POINTS);
	std::vector<Coordinate> ob1_coords = {	//must be counterclockwise
		{7*SPACE_UNIT, 	18*SPACE_UNIT},
		{12*SPACE_UNIT, 18*SPACE_UNIT},
		{12*SPACE_UNIT, 24*SPACE_UNIT},
		{7*SPACE_UNIT, 	24*SPACE_UNIT}};
	Polygon ob1(ob1_coords);
	m.add_obstacle(ob1);
	
	std::vector<Coordinate> ob2_coords = {	//must be counterclockwise
		{7*SPACE_UNIT, 	0*SPACE_UNIT},
		{12*SPACE_UNIT, 0*SPACE_UNIT},
		{12*SPACE_UNIT, 6*SPACE_UNIT},
		{7*SPACE_UNIT, 	6*SPACE_UNIT}};
	Polygon ob2(ob2_coords);
	m.add_obstacle(ob2);

	//Vehicle car(8*SPACE_UNIT, 3*SPACE_UNIT, Coordinate(4*SPACE_UNIT, 4*SPACE_UNIT), 0.4);
	while(1) {
		Vehicle car = Vehicle::random_vehicle();
		display_all(m, car);
	}
	end_graphics(display);

	return 0;
}