#ifndef VEHICLE_H
#define VEHICLE_H

#include "maneuver.h"
#include "map.h"
#include "util.h"

using namespace std;

/* A rectangular vehicle. */
class Vehicle {
    private:
        unsigned int length,  // length of the vehicle
            width;                  // width of the vehicle
        double orientation;         // vehicle's head facing angle (between 0 and pi)
        Coordinate rear_center,     // rear middle coordinate
            rear_left,              // rear left coordinate
            rear_right,             // rear right coordinate
            front_center,           // rear center coordinate
            front_left,             // front left coordinate
            front_right;            // front right coordinate

        /* Calculates the other coordinates starting only from rear middle coordinate and orientation. */
        void compute_secondary_coords();
        /* Adjusts the coordinates to align them to the middle of state. */
        void discretize_coords();
		
    public:
        /* getter methods */
        int get_length() const;
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

        /* Repositions the vehicle. This is like a teleport, it only checks for collisions
         * at the final position, not those occurring during the maneuvre (which is unknown).
         * Returns:     0 if success 
                        1 if the final position crashes with an obstacle 
                        2 if the final position is out of the map
                        3 if the final orientation is reversed */
        unsigned int reposition(Map& map, Coordinate pos, double angle);
        
        /* Moves the vehicle according to a specific maneuver. Also checks for collisions. 
         * Returns:	0 if success
         * 		1 if the vehicle crashed with an obstacle
         * 		2 if it went out of the map
         * 		3 if it steered too much (orientation overflow or underflow) */
        unsigned int move(Map& map, Maneuver& man);

        /* Generates a random vehicle. */
        static Vehicle random_vehicle();

        /* Returns a Polygon representation of this vehicle. */
        Polygon to_polygon() const;

        /* Decodes the state of a vehicle. Returns a 3-element vector:
         *      [0] -> orientation
         *      [1] -> x
         *      [2] -> y  */
        static std::vector<double> decode_vehicle(unsigned int code);
        
        /* Encodes the state of a vehicle */
        static unsigned int encode_vehicle(double orientation, double x, double y);
        
        /* Non-static version of encode_vehicle. */
        unsigned int encode();

        /* Constructor */
        Vehicle(unsigned int l, unsigned int w, Coordinate rc, double angle, bool discretize = true);
        
        /* Constructor*/
        Vehicle(unsigned int l, unsigned int w, unsigned int state_code);

        /* Copy constructor */
        Vehicle(const Vehicle& v1);
        
        /* Assignment operator */
        Vehicle& operator=(const Vehicle& v1);
        
        /* Ostream operator */
        friend std::ostream& operator<<(std::ostream& os, const Vehicle& v);
};

#endif

