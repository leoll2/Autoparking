#ifndef UTIL_H
#define UTIL_H

/* A (x,y) coordinate is equivalent to a direction */
#define Coordinate Direction

#include <cmath>

constexpr double pi = acos(-1);

class Direction {
	public:
		double x;
		double y;

		/* Vector sum */
		Direction operator+(const Direction& d1) const;
		
		/* Vector difference */
		Direction operator-(const Direction& d1) const;
		
		/* Vector inner product */
		double operator*(const Direction& d1) const;
		friend Direction operator*(const Direction& d1, const double a);
		friend Direction operator*(const double a, const Direction& d1);
		
		/* Get the angle between vector and positive x-axis.
		 * Range 0-2pi */
		double get_angle();
		Direction(double x, double y);
};

#endif