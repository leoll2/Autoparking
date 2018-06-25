#ifndef VEHICLE_H
#define VEHICLE_H

#include "maneuver.h"
#include "map.h"
#include "util.h"

using namespace std;


class Vehicle {
	private:
		const unsigned int length,
			width;
		double orientation;	//angle between 0 and pi
		Coordinate rear_center,
			rear_left,
			rear_right,
			front_center,
			front_left,
			front_right;
		void compute_secondary_coords();
		
	public:
		Coordinate get_rear_center() const;
		Coordinate get_rear_left() const;
		Coordinate get_rear_right() const;
		Coordinate get_front_center() const;
		Coordinate get_front_left() const;
		Coordinate get_front_right() const;
		float* get_vertices() const;
		bool verify_collision(const Map& m) const;
		unsigned int move(Map& map, Maneuver& man);
		static Vehicle random_vehicle();
		Vehicle(unsigned int l, unsigned int w, Coordinate rc, double angle);
};

#endif

