#ifndef MANEUVER_H
#define MANEUVER_H

#include <cstdint>
#include <iostream>

#define NUM_RADI		2
#define NUM_DISP		3
#define MIN_RADI		500
#define MIN_DISP		50

enum Verse : bool {
	FORWARD 	= true,
	BACKWARDS 	= false
};

enum Spin : int8_t {
	CLOCKWISE			= 1,
	STRAIGHT			= 0,
	COUNTERCLOCKWISE	= -1
};

enum TurningRadius : uint16_t {
	NARROW	= MIN_RADI,
	BROAD	= 2 * MIN_RADI
};

enum Displacement : uint16_t {
	SHORT	= 50,
	AVERAGE	= 100,
	LONG	= 150
};

class Maneuver {
	private:
		Verse verse;					// Forward or reverse gear
		Spin spin;						// Steer direction
		TurningRadius turning_radius;	// Steering radius
		Displacement displacement;		// Distance covered
		
		/* Decode a maneuver from its code. Initializes the attributes with proper values. */
		void decode_maneuver(unsigned int code);
	
	public:
		/* Encodes a maneuver. Returns its code. */
		unsigned int encode_maneuver();
		
		/* Prints the attributes of a maneuver. */
		friend std::ostream& operator<<(std::ostream& os, const Maneuver& m);
		
		/* Constructor */
		Maneuver(unsigned int code);
};

#endif
