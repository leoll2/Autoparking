#include "util.h"

/******************************************************************************/
/*        DIRECTION: a 2-d vector representing a cartesian direction          */
/******************************************************************************/

/**
 * Return a Direction object given an angle.
 * 
 * @param angle the given angle
 * @return the Direction corresponding to that angle
 */
Direction Direction::from_angle(double angle) {
    return Direction(std::cos(angle), std::sin(angle));
}


/**
 * Get the angle associated to a Direction.
 * 
 * @return the angle
 */
double Direction::get_angle() {
    double angle = std::atan2(y, x);
    if (angle < 0)
        return angle + 2 * pi;
    else
        return angle;
}


/**
 * Get the modulus of a vector.
 * 
 * @return the modulus of the given vector
 */
double Direction::get_modulus() {
    return std::sqrt(x * x + y * y);
}


/**
 * Get the normalized right perpendicular to a vector.
 * 
 * @return the normalized right perpendicular
 */
Direction Direction::get_right_perp() {
    double modulus = get_modulus();
    return (Direction(y, -x) * (1/modulus));
}


/**
 * Get the normalized left perpendicular to a vector.
 * 
 * @return the normalized left perpendicular
 */
Direction Direction::get_left_perp() {
    double modulus = get_modulus();
    return (Direction(-y, x) * (1/modulus));
}


/**
 * Rotate a point provided the rotation center and angle.
 * 
 * @param rot_center the rotation center
 * @param delta_angle the rotation angle
 * @return the Coordinate of rotated point
 */
Direction Direction::rotate(const Coordinate& rot_center, double delta_angle) {
    double radius = (*this - rot_center).get_modulus();
    double angle_from_rot_center = (*this - rot_center).get_angle();
    double new_angle_from_rot_center = wrap_angle(angle_from_rot_center + delta_angle);
    return rot_center + radius * Direction(std::cos(new_angle_from_rot_center), 
                                           std::sin(new_angle_from_rot_center));
}


/**
 * Vector addition.
 * 
 * @param d1 the vector to be added
 * @return the result of addition
 */
Direction Direction::operator+(const Direction& d1) const {
    return Direction(x + d1.x, y + d1.y);
}


/**
 * Vector difference.
 * 
 * @param d1 the vector to be subtracted
 * @return the result of difference
 */
Direction Direction::operator-(const Direction& d1) const {
    return Direction(x - d1.x, y - d1.y);
}


/**
 * Vector inner product.
 * 
 * @param d1 the multiplying vector
 * @return the result of inner product
 */
double Direction::operator*(const Direction& d1) const {
    return x * d1.x + y * d1.y;
}


/**
 * Vector scalar multiplication.
 * 
 * @param d1 the vector to be scaled
 * @param a the scaling factor
 * @return the scaled vector
 */
Direction operator*(const Direction& d1, const double a) {
    return Direction(d1.x * a, d1.y * a);
}


/**
 * Vector scalar multiplication.
 * 
 * @param d1 the vector to be scaled
 * @param a the scaling factor
 * @return the scaled vector
 */
Direction operator*(const double a, const Direction& d1) {
    return d1 * a;
}


/**
 * A 2-D direction vector provided its x and y components.
 * 
 * @param x the x component
 * @param y the y component
 */
Direction::Direction(double x, double y) : x(x), y(y) {};


/**
 * Ostream operator for Direction.
 * 
 * @param os the ostream
 * @param d the Direction
 * @return the ostream
 */
std::ostream& operator<<(std::ostream& os, const Direction& d) {
    os << "(" << d.x << ", " << d.y << ")";
    return os;
}