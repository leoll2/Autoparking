#ifndef POLYGON_H
#define POLYGON_H

#include <cstdlib>
#include <vector>
#include "util.h"

using namespace std;

class Polygon;	//forward declaration

/* From theta1 to theta2 counterclockwise */
class Arc {
private:
    const Coordinate center;
    const double radius,
        theta1,
        theta2;
    public:
        Arc(Coordinate c, double r, double t1, double t2);
        friend bool check_collision(const Arc& a, const Polygon& p);
        friend bool check_collision(const Polygon& p, const Arc& a);
        /* Ostream operator */
	friend std::ostream& operator<<(std::ostream& os, const Arc& a);
};

class Polygon {
    private:
        const unsigned int n_sides;
        const vector<Coordinate> vertices;
    public:
        Polygon(const vector<Coordinate> &v);
        unsigned int get_n_sides() const;
        vector<Coordinate> get_vertices1() const;
        float* get_vertices2() const;
        friend bool _check_collision(const Polygon& p1, const Polygon& p2);
        friend bool check_collision(const Polygon& p1, const Polygon& p2);
        friend bool check_collision(const Arc& a, const Polygon& p);
        friend bool check_collision(const Polygon& p, const Arc& a);
        Polygon& operator=(const Polygon& p1);
};

#endif