#include "shapes.h"

Arc::Arc(Coordinate c, double r, double t1, double t2) :
	center(c), radius(r), theta1(t1), theta2(t2) {};

std::ostream& operator<<(std::ostream& os, const Arc& a) {
	os << "Arc[c: " << a.center << ", r: " << a.radius << ", theta1: " << a.theta1 << ", theta2: " << a.theta2 << "]";
	return os;
}
        
unsigned int Polygon::get_n_sides() const {
	return n_sides;
}

vector<Coordinate> Polygon::get_vertices1() const {
	return vertices;
}

float* Polygon::get_vertices2() const {
	float *ret = (float*) malloc(2 * n_sides * sizeof(float));
	for (unsigned int i = 0; i < n_sides; ++i) {
		ret[2*i] 	= vertices[i].x;
		ret[2*i+1] 	= vertices[i].y;
	}
	return ret;
}

Polygon::Polygon(const vector<Coordinate> &v) :
	n_sides(v.size()), vertices(v) { }

/* Note: the case "arc enclosed by the polygon" isn't yet considered
 * nor implemented. May not be needed for our purposes.*/
bool check_collision(const Arc& arc, const Polygon& p) {
    // For each segment of p
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
            // If the first intersection point falls in the segment
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
            // If the second intersection point falls in the segment
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