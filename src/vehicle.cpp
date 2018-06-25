#include <cstdlib>
#include <ctime>
#include <iostream>
#include "field_params.h"
#include "vehicle.h"

using namespace std;

Coordinate Vehicle::get_rear_center() const {
	return rear_center;
}

Coordinate Vehicle::get_rear_left() const {
	return rear_left;
}

Coordinate Vehicle::get_rear_right() const {
	return rear_right;
}

Coordinate Vehicle::get_front_center() const {
	return front_center;
}

Coordinate Vehicle::get_front_left() const {
	return front_left;
}

Coordinate Vehicle::get_front_right() const {
	return front_right;
}

float* Vehicle::get_vertices() const {
	float *ret = (float*) malloc(8 * sizeof(float));
	ret[0] = rear_left.x;
	ret[1] = rear_left.y;
	ret[2] = rear_right.x;
	ret[3] = rear_right.y;
	ret[4] = front_right.x;
	ret[5] = front_right.y;
	ret[6] = front_left.x;
	ret[7] = front_left.y;
	return ret;
}

void Vehicle::compute_secondary_coords() {
	rear_left  = 	Coordinate((rear_center.x - (width/2 * sin(orientation))) , (rear_center.y + (width/2 * cos(orientation))));
	rear_right =  	Coordinate((rear_center.x + (width/2 * sin(orientation))) , (rear_center.y - (width/2 * cos(orientation))));
	front_center =	Coordinate((rear_center.x + (length  * cos(orientation))) , (rear_center.y + (length  * sin(orientation))));
	front_left = 	Coordinate((front_center.x - (width/2 * sin(orientation))) , (front_center.y + (width/2 * cos(orientation))));
	front_right =	Coordinate((front_center.x + (width/2 * sin(orientation))) , (front_center.y - (width/2 * cos(orientation))));
}

Vehicle::Vehicle(unsigned int l, unsigned int w, Coordinate rc, double angle) :
	length(l),
	width(w),
	orientation(angle),
	rear_center(rc),
	rear_left((rear_center.x - (width/2 * sin(orientation))) , (rear_center.y + (width/2 * cos(orientation)))),
	rear_right((rear_center.x + (width/2 * sin(orientation))) , (rear_center.y - (width/2 * cos(orientation)))),
	front_center((rear_center.x + (length  * cos(orientation))) , (rear_center.y + (length  * sin(orientation)))),
	front_left((front_center.x - (width/2 * sin(orientation))) , (front_center.y + (width/2 * cos(orientation)))),
	front_right((front_center.x + (width/2 * sin(orientation))) , (front_center.y - (width/2 * cos(orientation))))
	{ /*
		std::cout << "created vehicle" << endl;
		std::cout << "RL: (" << rear_left.x << ", " << rear_left.y << ")" << endl;
		std::cout << "RR: (" << rear_right.x << ", " << rear_right.y << ")" << endl;
		std::cout << "FL: (" << front_left.x << ", " << front_left.y << ")" << endl;
		std::cout << "FR: (" << front_right.x << ", " << front_right.y << ")" << endl;*/
	}

bool Vehicle::verify_collision(const Map& m) const {
	std::vector<Coordinate> coords = {rear_left, rear_right, front_right, front_left};
	Polygon car(coords);
	for (auto ob : m.obstacles)
		if (check_collision(ob, car))
			return true;
	return false;
}

/* Return 0 if the move was successful. 
   Return 1 if the vehicle crashed with an object.
   Return 2 if the vehicle went out of the map.*/
unsigned int Vehicle::move(Map& map, Maneuver& man) {
	//CALCULATE NEW COORDS
	//CHECK FOR OUT OF MAP
	return 2;
	//CHECK FOR COLLISION WITH OBJECT
	return 1;
	//UPDATE POSITION
	//orientation = new_orientation;
	//rear_center = new_rear_center;
	//compute_secondary_coords();
	return 0;
}

Vehicle Vehicle::random_vehicle() {
	srand(time(NULL));
	int random_x = rand() % (HREF_POINTS * SPACE_UNIT);
	int random_y = rand() % (VREF_POINTS * SPACE_UNIT);
	float random_angle = ((double)rand() / RAND_MAX) * (2*pi);
	return Vehicle(8*SPACE_UNIT, 3*SPACE_UNIT, Coordinate(random_x, random_y), random_angle);
}