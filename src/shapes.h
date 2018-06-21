#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "util.h"

using namespace std;

class Arc {
	private:
		const double radius,
			theta1,
			theta2;
	public:
		Arc(double r, double t1, double t2);
		friend bool check_collision(Arc& a, Polygon& p);
		friend bool check_collision(Polygon& p, Arc& a);
		
};

class Polygon {
	
	private:
		const unsigned int n_sides;				
		const vector<Coordinate> vertices;
	public:
		Polygon(int n, const vector<Coordinate> &v);
		friend bool check_collision(Polygon& p1, Polygon& p2);
		friend bool check_collision(Arc& a, Polygon& p);
		friend bool check_collision(Polygon& p, Arc& a);
};

#endif