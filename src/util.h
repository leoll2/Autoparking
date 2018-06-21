#ifndef UTIL_H
#define UTIL_H

#define Coordinate Direction

struct Direction {
	int x;
	int y;
};

double dot(const Direction& a, const Direction& b);

#endif