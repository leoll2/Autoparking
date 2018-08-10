#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "field_params.h"
#include "vehicle.h"

using namespace std;

int	Vehicle::get_length() const {
	return length;
}

int Vehicle::get_width() const {
	return width;
}

double Vehicle::get_orientation() const {
	return orientation;
}

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

std::vector<Coordinate> Vehicle::get_vertices1() const {
	std::vector<Coordinate> ret;
	ret.push_back(rear_left);
	ret.push_back(rear_right);
	ret.push_back(front_right);
	ret.push_back(front_left);
	return ret;
}

float* Vehicle::get_vertices2() const {
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
	orientation(wrap_angle(angle)),
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
	
Vehicle::Vehicle(const Vehicle& v1) :
	length(v1.get_length()),
	width(v1.get_width()),
	orientation(v1.get_orientation()),
	rear_center(v1.get_rear_center()),
	rear_left(v1.get_rear_left()),
	rear_right(v1.get_rear_right()),
	front_center(v1.get_front_center()),
	front_left(v1.get_front_left()),
	front_right(v1.get_front_right()) {
}

bool Vehicle::verify_collision(const Map& m) const {
	std::vector<Coordinate> coords = {rear_left, rear_right, front_right, front_left};
	Polygon car(coords);
	for (auto ob : m.obstacles)
		if (check_collision(ob, car))
			return true;
	return false;
}

Coordinate compute_rotation_center(Coordinate vehicle, double orientation, TurningRadius r, Spin s, Verse v) {
	assert (s == CLOCKWISE || s == COUNTERCLOCKWISE);

	Direction head = Direction::from_angle(orientation);
	if ((s == CLOCKWISE && v == FORWARD) || (s == COUNTERCLOCKWISE && v == BACKWARDS))
		return vehicle + (head.get_right_perp() * (double)r);
	else
		return vehicle + (head.get_left_perp() * (double)r);
}

unsigned int Vehicle::move(Map& map, Maneuver& mnv) {
	// Save the current state of the vehicle (ghost)
	Vehicle ghost(*this);
	
	// Update position and orientation
	Verse verse = mnv.get_verse();
	Spin spin = mnv.get_spin();
	Displacement arc_length = mnv.get_displacement();
	switch(spin) {
		case STRAIGHT:
			rear_center = rear_center + verse * Direction::from_angle(orientation) * arc_length;
			break;
		case CLOCKWISE:
		case COUNTERCLOCKWISE:
			TurningRadius radius = mnv.get_turning_radius();
			Coordinate rot_center = compute_rotation_center(rear_center, orientation, radius, spin, verse);
			double angle_from_rot_center = (rear_center - rot_center).get_angle();
			double delta_angle;
			if (spin == CLOCKWISE)
				delta_angle = - double(arc_length) / radius;
			else
				delta_angle = double(arc_length) / radius;
			
			double new_angle_from_rot_center = wrap_angle(angle_from_rot_center + delta_angle);
			rear_center = rot_center + radius * Direction(cos(new_angle_from_rot_center), sin(new_angle_from_rot_center));
			orientation = wrap_angle(orientation + delta_angle);
	}
	compute_secondary_coords();
	
	// Verify if the vehicle is still well-aligned (faces an angle between 0 and pi)
	if (orientation > pi)
		return 3;
	
	// Verify if the vehicle is still inside the map
	Polygon p = this->to_polygon();
	if (!map.is_within_boundaries(p))
		return 2;
	
	// Check if the vehicle collided with any obstacle
	if (map.collides_with_obstacles(p))
		return 1;
	// TO BE IMPLEMENTED: VERIFICA COLLISIONE ARCHI!!!

	return 0;
}

Vehicle Vehicle::random_vehicle() {
	srand(time(NULL));
	int random_x = rand() % (HREF_POINTS * SPACE_UNIT);
	int random_y = rand() % (VREF_POINTS * SPACE_UNIT);
	float random_angle = ((double)rand() / RAND_MAX) * (2*pi);
	return Vehicle(8*SPACE_UNIT, 3*SPACE_UNIT, Coordinate(random_x, random_y), random_angle);
}

Polygon Vehicle::to_polygon() const {
	return Polygon(this->get_vertices1());
}