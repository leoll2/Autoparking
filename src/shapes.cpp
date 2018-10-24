#include "shapes.h"

/******************************************************************************/
/*                  ARC: a geometric arc of circumference                     */
/******************************************************************************/

/**
 * An arc of circumference, provided the angles with respect to the center in
 * counterclockwise order.
 * 
 * @param c the circumference center
 * @param r the circumference radius
 * @param t1 the first angle
 * @param t2 the second angle
 */
Arc::Arc(Coordinate c, double r, double t1, double t2) :
	center(c), radius(r), theta1(t1), theta2(t2) {};

        
/**
 * Ostream operator for Arc.
 * 
 * @param os the ostream object
 * @param a the Arc
 * @return the ostream object
 */
std::ostream& operator<<(std::ostream& os, const Arc& a) {
	os << "Arc[c: " << a.center << ", r: " << a.radius << ", theta1: " << a.theta1 << ", theta2: " << a.theta2 << "]";
	return os;
}

/******************************************************************************/
/*                      POLYGON: a geometric polygon                          */
/******************************************************************************/

/**
 * Get the number of sides of the polygon.
 * 
 * @return the number of sides
 */
unsigned int Polygon::get_n_sides() const {
    return n_sides;
}


/**
 * Get the coordinates of the vertices as a vector.
 * 
 * @return a vector of Coordinate
 */
std::vector<Coordinate> Polygon::get_vertices1() const {
    return vertices;
}


/**
 * Get the coordinates of the vertices as an array of interleaved x and y.
 * 
 * @return an array of interleaved x,y coordinates
 */
float* Polygon::get_vertices2() const {
    float *ret = (float*) malloc(2 * n_sides * sizeof(float));
    for (unsigned int i = 0; i < n_sides; ++i) {
        ret[2*i]  = vertices[i].x;
        ret[2*i+1]= vertices[i].y;
    }
    return ret;
}


/**
 * A polygon, that is a plane figure bounded by a finite chain of straight segments.
 * 
 * @param v the vector containing the coordinates of the polygon (counterclockwisely)
 */
Polygon::Polygon(const std::vector<Coordinate> &v) : n_sides(v.size()), vertices(v) { }



/**
 * Check if an arc and a polygon are colliding on their borders, that is overlap
 * at least on one point. Note: if the arc is entirely enclosed by the polygon,
 * they are not considered colliding.
 * 
 * @param arc the Arc
 * @param p the Polygon
 * @return true if colliding, false otherwise
 */
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
    // OPTIONAL IMPLEMENTATION HERE: Check if the arc is completely inside the polygon
    return false;
}


/**
 * Check if an arc and a polygon are colliding on their borders, that is overlap
 * at least on one point. Note: if the arc is entirely enclosed by the polygon,
 * they are not considered colliding.
 * 
 * @param p the Polygon
 * @param arc the Arc
 * @return true if colliding, false otherwise
 */
bool check_collision(const Polygon& p, const Arc& a) {
    return check_collision(a, p);
}


/**
 * Uses the SAT algorithm to test the edges of the first polygon. This function
 * has to be called twice (with swapped arguments) to truly check for collisions.
 * 
 * @param p1 the first polygon
 * @param p2 the second polygon
 * @return true 
 */
bool _check_collision(const Polygon& p1, const Polygon& p2) {
    
    // For each segment of p1
    for (unsigned int i = 0; i < p1.n_sides; ++i) {
        const Direction edge(p1.vertices[i].x - p1.vertices[(i + 1) % p1.n_sides].x,
                             p1.vertices[i].y - p1.vertices[(i + 1) % p1.n_sides].y);
        
        // Find the normal vector
        Direction normal = {edge.y, -edge.x};
        
        // Project all the vertices of p1 onto the normal
        double min1 = p1.vertices[0] * normal;	//inner product
        double max1 = min1;
        for (unsigned int j = 1; j < p1.n_sides; ++j) {
            double proj = p1.vertices[j] * normal;
            if (proj < min1)
                min1 = proj;
            else if (proj > max1)
                max1 = proj;
        }
        
        // Project all vertices of p2 onto the normal
        double min2 = p2.vertices[0] * normal;	//inner product
        double max2 = min2;
        for (unsigned int j = 1; j < p2.n_sides; ++j) {
            double proj = p2.vertices[j] * normal;
            if (proj < min2)
                min2 = proj;
            else if (proj > max2)
                max2 = proj;
        }
        
        // Return false if there is a gap between the two shapes
        if ((min1 < min2 && max1 < min2) || (min2 < min1 && max2 < min1))
            return false;
    }
    return true;
}

/**
 * Check if two polygons are colliding using SAT (Separating Axis Theorem).
 * 
 * @param p1 the first polygon
 * @param p2 the second polygon
 * @return true if they overlap, false otherwise
 */
bool check_collision(const Polygon& p1, const Polygon& p2) {
    return _check_collision(p1, p2) && _check_collision(p2, p1);
}