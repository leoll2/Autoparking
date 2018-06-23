#include "shapes.h"

Arc::Arc(Coordinate c, double r, double t1, double t2) :
	center(c), radius(r), theta1(t1), theta2(t2) {};

Polygon::Polygon(const vector<Coordinate> &v) :
	n_sides(v.size()), vertices(v) { }

/* Note: the case "arc enclosed by the polygon" isn't yet considered
 * nor implemented. May not be needed for our purposes.*/
bool check_collision(const Arc& arc, const Polygon& p) {
	///For each segment of p
	for (unsigned int i = 0; i < p.n_sides; ++i) {
		const Direction edge = p.vertices[(i + 1) % p.n_sides] - p.vertices[i];
		const Direction f = p.vertices[i] - arc.center;
		
		double a = edge * edge;
		double b = 2 * (f * edge);
		double c = (f * f) - (arc.radius * arc.radius);
		
		double delta = b*b - 4*a*c;
		if (delta < 0) {
			continue;
		} else {
			delta = sqrt(delta);
			double t1 = (-b - delta) / (2*a);
			double t2 = (-b + delta) / (2*a);
			//if the first intersection point falls in the segment
			if (t1 >= 0 && t1 <= 1) {
				Coordinate cross_point = p.vertices[i] + t1 * edge;
				double cross_point_angle = (cross_point - arc.center).get_angle();
				if (arc.theta1 <= arc.theta2) {
					if ((arc.theta1 < cross_point_angle) && (cross_point_angle < arc.theta2)) {
						return true;
					}
				} else {
					if ((arc.theta1 < cross_point_angle) || (cross_point_angle < arc.theta2)) {
						return true;
					}
				}
			}
			//if the second intersection point falls in the segment
			if (t2 >= 0 && t2 <= 1) {
				Coordinate cross_point = p.vertices[i] + t2 * edge;
				double cross_point_angle = (cross_point - arc.center).get_angle();
				if (arc.theta1 <= arc.theta2) {
					if ((arc.theta1 < cross_point_angle) && (cross_point_angle < arc.theta2)) {
						return true;
					}
				} else {
					if ((arc.theta1 < cross_point_angle) || (cross_point_angle < arc.theta2)) {
						return true;
					}
				}
			}
		}
	}
	//TO BE IMPLEMENTED HERE: CHECK IF WHOLE ARC IS INSIDE THE POLYGON
	return false;
}

/*	Old code (wrong): works only for convex figures, i.e. when the arc
 *  is seen "convex" from the polygon. Some of this code may still be
 *  useful to implement the check "arc enclosed in the polygon".
 * 
 * bool check_collision(const Arc& a, const Polygon& p) {
	///For each segment of p1
	for (unsigned int i = 0; i < p.n_sides; ++i) {
		const Direction edge(p.vertices[i].x - p.vertices[(i + 1) % p.n_sides].x,
							p.vertices[i].y - p.vertices[(i + 1) % p.n_sides].y);
		//find the normal vector
		Direction normal(edge.y, -edge.x);
		//project all vertices of p onto the normal
		double min1 = p.vertices[0] * normal;
		double max1 = min1;
		for (unsigned int j = 1; j < p.n_sides; ++j) {
			double proj = p.vertices[j] * normal;
			if (proj < min1)
				min1 = proj;
			else if (proj > max1)
				max1 = proj;
		}
		//PROIETTA I DUE PUNTI DELL'ARCO
		Coordinate a1 = a.center + Coordinate(a.radius*cos(a.theta1), a.radius*sin(a.theta1));
		Coordinate a2 = a.center + Coordinate(a.radius*cos(a.theta2), a.radius*sin(a.theta2));
		double min2 = a1 * normal;
		double max2 = min2;
		double proj = a2 * normal;
		if (proj < min2)
			min2 = proj;
		else if (proj > max2)
			max2 = proj;
		//PROIETTA CENTRO +- RADIUS SOLO SE APPARTIENE ALL'ARCO (verifica angolo)
		double normal_angle1 = normal.get_angle();
		double normal_angle2 = fmod((normal_angle1 + pi), 2*pi);	//supplementary
		Coordinate ext1 = a.center + Coordinate(a.radius*cos(normal_angle1), a.radius*sin(normal_angle1));
		Coordinate ext2 = a.center + Coordinate(a.radius*cos(normal_angle2), a.radius*sin(normal_angle2));
		double proj1 = ext1 * normal;
		double proj2 = ext2 * normal;
		
		if (a.theta1 <= a.theta2) {
			if ((a.theta1 < normal_angle1) && (normal_angle1 < a.theta2)) {
				if (proj1 < min2)
					min2 = proj1;
				else if (proj1 > max2)
					max2 = proj1;
			}
			if ((a.theta1 < normal_angle2) && (normal_angle2 < a.theta2)) {
				if (proj2 < min2)
					min2 = proj2;
				else if (proj2 > max2)
					max2 = proj2;
			}
		} else {
			if ((a.theta1 < normal_angle1) || (normal_angle1 < a.theta2)) {
				if (proj1 < min2)
					min2 = proj1;
				else if (proj1 > max2)
					max2 = proj1;
			}
			if ((a.theta1 < normal_angle2) || (normal_angle2 < a.theta2)) {
				if (proj2 < min2)
					min2 = proj2;
				else if (proj2 > max2)
					max2 = proj2;
			}
		}
		//return true if there's no gap between the two shapes
		if ((min1 < min2 && max1 < min2) || (min2 < min1 && max2 < min1))
			return false;
	}
	return true;
}*/

bool check_collision(const Polygon& p, const Arc& a) {
	return check_collision(a, p);
}

/* Applies the SAT algorithm and tests the edges of the first polygon.
 * Needs to be called twice (with swapped arguments) to truly check for
 * collisions.*/
bool _check_collision(const Polygon& p1, const Polygon& p2) {
	//For each segment of p1
	for (unsigned int i = 0; i < p1.n_sides; ++i) {
		const Direction edge(p1.vertices[i].x - p1.vertices[(i + 1) % p1.n_sides].x,
							p1.vertices[i].y - p1.vertices[(i + 1) % p1.n_sides].y);
		//find the normal vector
		Direction normal = {edge.y, -edge.x};
		//project all vertices of p1 onto the normal
		double min1 = p1.vertices[0] * normal;	//inner product
		double max1 = min1;
		for (unsigned int j = 1; j < p1.n_sides; ++j) {
			double proj = p1.vertices[j] * normal;
			if (proj < min1)
				min1 = proj;
			else if (proj > max1)
				max1 = proj;
		}
		//project all vertices of p2 onto the normal
		double min2 = p2.vertices[0] * normal;	//inner product
		double max2 = min2;
		for (unsigned int j = 1; j < p2.n_sides; ++j) {
			double proj = p2.vertices[j] * normal;
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

/* SAT algorithm. Returns true if the objects are colliding. */
bool check_collision(const Polygon& p1, const Polygon& p2) {
	return _check_collision(p1, p2) && _check_collision(p2, p1);
}