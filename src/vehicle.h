#ifndef VEHICLE_H
#define VEHICLE_H

#include "maneuver.h"
#include "map.h"
#include "util.h"

using namespace std;

/* A rectangular vehicle. */
class Vehicle {
	private:
		const unsigned int length,	// length of the vehicle
			width;					// width of the vehicle
		double orientation;			// vehicle's head facing angle (between 0 and pi)
		Coordinate rear_center,		// rear middle coordinate
			rear_left,				// rear left coordinate
			rear_right,				// rear right coordinate
			front_center,			// rear center coordinate
			front_left,				// front left coordinate
			front_right;			// front right coordinate
			
		/* Calculate the other coordinates starting only from rear middle coordinate and orientation. */
		void compute_secondary_coords();
		
public:
		/* getter methods */
		int	get_length() const;
		int get_width() const;
		double get_orientation() const;
		Coordinate get_rear_center() const;
		Coordinate get_rear_left() const;
		Coordinate get_rear_right() const;
		Coordinate get_front_center() const;
		Coordinate get_front_left() const;
		Coordinate get_front_right() const;
		
		/* Get the coordinates of the four vertices as Coordinate objects.
		 * Starts from rear left and goes counter-clockwise. */
		std::vector<Coordinate> get_vertices1() const;
		
		/* Get the coordinates of the four vertices decomposed into x and y float components. 
		 * Starts from rear left and goes counter-clockwise. 
		 * Even indices are 'x' coordinates, odd ones are 'y'. */
		float* get_vertices2() const;
		
		/* Returns true if the vehicle collides with at least one obstacle of the map. */
		bool verify_collision(const Map& m) const;
		
		/* Moves the vehicle to a new specified position. 
		 * Returns:	0 if success
		 * 			1 if the vehicle crashed with an obstacle
		 * 			2 if it went out of the map
		 * 			3 if it steered too much (orientation overflow or underflow) */
		unsigned int move(Map& map, Maneuver& man);
		
		/* Generate a random vehicle. */
		static Vehicle random_vehicle();
		
		/* Return a Polygon representation of this vehicle. */
		Polygon to_polygon();
		
		/* Constructor */
		Vehicle(unsigned int l, unsigned int w, Coordinate rc, double angle);
		
		/* Copy constructor */
		Vehicle(const Vehicle& v1);
};

#endif

