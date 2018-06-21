#include "shapes.h"

Arc::Arc(double r, double t1, double t2) :
	radius(r), theta1(t1), theta2(t2) {};

Polygon::Polygon(int n, const vector<Coordinate> &v) :
	n_sides(n), vertices(v) { }


bool check_collision(Arc& a, Polygon& p) {
	/*TO BE IMPLEMENTED*/
	return false;
}

bool check_collision(Polygon& p, Arc& a) {
	return check_collision(a, p);
}

/* SAT algorithm. Returns true if the objects are colliding. */
bool check_collision(Polygon& p1, Polygon& p2) {
	//For each segment of p1
	for (unsigned int i = 0; i < p1.n_sides; ++i) {
		const Direction edge = {p1.vertices[i].x - p1.vertices[(i + 1) % p1.n_sides].x,
								p1.vertices[i].y - p1.vertices[(i + 1) % p1.n_sides].y};
		//find the normal vector
		Direction normal = {edge.y, -edge.x};
		//project all vertices of p1 onto the normal
		double min1 = dot(p1.vertices[0], normal);
		double max1 = min1;
		for (unsigned int j = 1; j < p1.n_sides; ++j) {
			double proj = dot(p1.vertices[j], normal);
			if (proj < min1)
				min1 = proj;
			else if (proj > max1)
				max1 = proj;
		}
		//project all vertices of p2 onto the normal
		double min2 = dot(p2.vertices[0], normal);
		double max2 = min2;
		for (unsigned int j = 1; j < p2.n_sides; ++j) {
			double proj = dot(p2.vertices[j], normal);
			if (proj < min2)
				min2 = proj;
			else if (proj > max2)
				max2 = proj;
		}
		//return true if there's no gap between the two shapes
		if ((min1 < min2 && max1 < min2) || (min2 < min1 && max2 < min1))
			return false;
	}
	return true;
}