#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <iostream>

/* A (x,y) coordinate is equivalent to a direction */
#define Coordinate Direction

/* Constant 'pi'. */
constexpr double pi = acos(-1);

/* Given an angle in radians (any range), converts it into range [0,2pi). */
inline double wrap_angle(double angle) {
	angle = fmod(angle, 2.0 * pi);
	if (angle < 0.0)
	   angle += 2.0 * pi;
	return angle;
}

/* Cartesian direction (two-dimensions) */
class Direction {
	
	public:
		double x;
		double y;

		/* Converts an angle into a direction. The returned vector is normalized.*/
		static Direction from_angle(double angle);
		
		/* Get the angle between vector and positive x-axis.
		 * Range 0-2pi */
		double get_angle();

		/* Return the modulus of the direction vector. */
		double get_modulus();

		/* Get the right-hand perpendicular direction. The returned vector is normalized.*/
		Direction get_right_perp();
		
		/* Get the left-hand perpendicular direction. The returned vector is normalized.*/
		Direction get_left_perp();

		/* Vector sum */
		Direction operator+(const Direction& d1) const;
		
		/* Vector difference */
		Direction operator-(const Direction& d1) const;
		
		/* Vector inner product */
		double operator*(const Direction& d1) const;
		friend Direction operator*(const Direction& d1, const double a);
		friend Direction operator*(const double a, const Direction& d1);
		
		/* Ostream operator */
		friend std::ostream& operator<<(std::ostream& os, const Direction& d);
		
		/* Constructor */
		Direction(double x, double y);
};

#endif