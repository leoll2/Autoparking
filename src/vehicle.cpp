#include "vehicle.h"

using namespace std;

Coordinate Vehicle::get_rear_center() {
	return rear_center;
}

Coordinate Vehicle::get_rear_left() {
	return rear_left;
}

Coordinate Vehicle::get_rear_right() {
	return rear_right;
}

Coordinate Vehicle::get_front_center() {
	return front_center;
}

Coordinate Vehicle::get_front_left() {
	return front_left;
}

Coordinate Vehicle::get_front_right() {
	return front_right;
}

void compute_secondary_coords() {
	rear_left  = 	{(rc.x - (w/2 * sin(orientation)) , (rc.y + (w/2 * cos(orientation)))};
	rear_right =  	{(rc.x + (w/2 * sin(orientation)) , (rc.y - (w/2 * cos(orientation)))};
	front_center =	{(rc.x + (l   * cos(orientation)) , (rc.y + (l   * sin(orientation)))};
	front_left = 	{(front_center.x - (w/2 * sin(orientation)) , (front_center.y + (w/2 * cos(orientation)))};
	front_right =	{(front_center.x + (w/2 * sin(orientation)) , (front_center.y - (w/2 * cos(orientation)))}
}

Vehicle::Vehicle(unsigned int l, unsigned int w, Coordinate rc, int angle) {
	orientation = angle;
	length = l;
	width = w;
	rear_center = 	rc;
	compute_secondary_coords();
}

/* Return 0 if the move was successful. 
   Return 1 if the vehicle crashed with an object.
   Return 2 if the vehicle went out of the map.*/
unsigned int Vehicle::move(Map& map, Maneuver& man) {
	//CALCULATE NEW COORDS
	//CHECK FOR OUT OF MAP
	return 2
	//CHECK FOR COLLISION WITH OBJECT
	return 1
	//UPDATE POSITION
	orientation = new_orientation;
	rear_center = new_rear_center;
	compute_secondary_coords();
	return 0
}
