#ifndef UTIL_H
#define UTIL_H

#define Coordinate Direction

#include <cmath>

constexpr double pi = acos(-1);

class Direction {
	public:
		double x;
		double y;

		Direction operator+(const Direction& d1) const;
		Direction operator-(const Direction& d1) const;
		double operator*(const Direction& d1) const;
		friend Direction operator*(const Direction& d1, const double a);
		friend Direction operator*(const double a, const Direction& d1);
		double get_angle();	//range 0-2pi
		Direction(double x, double y);
};

//double dot(const Direction& a, const Direction& b);

#endif