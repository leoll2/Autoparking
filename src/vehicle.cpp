#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "field_params.h"
#include "vehicle.h"
#include "vehicle_params.h"

/******************************************************************************/
/*        VEHICLE: a vehicle with rectangular body and front steering         */
/******************************************************************************/

/**
 * Get the longitudinal length of the vehicle.
 * 
 * @return the length
 */
int Vehicle::get_length() const {
    return length;
}


/**
 * Get the width of the vehicle.
 * 
 * @return the width
 */
int Vehicle::get_width() const {
    return width;
}


/**
 * Get the angle of orientation of the vehicle.
 * 
 * @return the orientation angle
 */
double Vehicle::get_orientation() const {
    return orientation;
}


/**
 * Get the coordinate of the rear axis center.
 * 
 * @return the Coordinate of rear center
 */
Coordinate Vehicle::get_rear_center() const {
    return rear_center;
}


/**
 * Get the coordinate of the rear left corner.
 * 
 * @return the Coordinate of the rear left
 */
Coordinate Vehicle::get_rear_left() const {
    return rear_left;
}


/**
 * Get the coordinate of the rear right corner.
 * 
 * @return the Coordinate of the rear right
 */
Coordinate Vehicle::get_rear_right() const {
    return rear_right;
}


/**
 * Get the coordinate of the front axis center.
 * 
 * @return the Coordinate of the front center
 */
Coordinate Vehicle::get_front_center() const {
    return front_center;
}


/**
 * Get the coordinate of the front left corner.
 * 
 * @return the Coordinate of the front left
 */
Coordinate Vehicle::get_front_left() const {
    return front_left;
}


/**
 * Get the coordinate of the front right corner.
 * 
 * @return the Coordinate of the front right
 */
Coordinate Vehicle::get_front_right() const {
    return front_right;
}


/**
 * Get the vertices of the car as a vector of Coordinate.
 * 
 * @return the vector containing the coordinates
 */
std::vector<Coordinate> Vehicle::get_vertices1() const {
    
    std::vector<Coordinate> ret;
    ret.push_back(rear_left);
    ret.push_back(rear_right);
    ret.push_back(front_right);
    ret.push_back(front_left);
    return ret;
}


/**
 * Get the vertices of the car as an array of interleaved x and y coordinates.
 * 
 * @return the array containing the coordinates
 */
float* Vehicle::get_vertices2() const {
    
    float *ret = (float*) malloc(8 * sizeof(float));
    ret[0] = rear_left.x;
    ret[1] = rear_left.y;
    ret[2] = rear_right.x;
    ret[3] = rear_right.y;
    ret[4] = front_right.x;
    ret[5] = front_right.y;
    ret[6] = front_left.x;
    ret[7] = front_left.y;
    return ret;
}


/**
 * Compute the coordinates of the vehicle corners starting from the rear center
 * and orientation.
 */
void Vehicle::compute_secondary_coords() {
    
    rear_left  =    Coordinate((rear_center.x - (width/2 * sin(orientation))) , 
                               (rear_center.y + (width/2 * cos(orientation))));
    rear_right =    Coordinate((rear_center.x + (width/2 * sin(orientation))) , 
                               (rear_center.y - (width/2 * cos(orientation))));
    front_center =  Coordinate((rear_center.x + (length  * cos(orientation))) , 
                               (rear_center.y + (length  * sin(orientation))));
    front_left =    Coordinate((front_center.x - (width/2 * sin(orientation))) , 
                               (front_center.y + (width/2 * cos(orientation))));
    front_right =   Coordinate((front_center.x + (width/2 * sin(orientation))) , 
                               (front_center.y - (width/2 * cos(orientation))));
}


/**
 * Slightly adjust the coordinates of the vehicle to match the closest state.
 */
void Vehicle::discretize_coords() {
    
    std::vector<double> state = decode_vehicle(this->encode());
    rear_center = Coordinate(state[1], state[2]);
    orientation = state[0];
    compute_secondary_coords();
}


/**
 * A rectangular vehicle with custom dimensions and position
 * 
 * @param l the length of the body
 * @param w the width of the body
 * @param rc the Coordinate of rear axis center
 * @param angle the orientation angle
 * @param discretize true to match the closest state, false otherwise
 */
Vehicle::Vehicle(unsigned int l, unsigned int w, Coordinate rc, double angle, bool discretize) :
    length(l),
    width(w),
    orientation(wrap_angle(angle)),
    rear_center(rc),
    rear_left(0,0),
    rear_right(0,0),
    front_center(0,0),
    front_left(0,0),
    front_right(0,0)
{
    if (discretize)
        discretize_coords();
    else
        compute_secondary_coords();
}


/**
 * A rectangular vehicle with custom dimension and placed in a specified state.
 * 
 * @param l the length of the body
 * @param w the width of the body
 * @param state_code the code of the state
 */
Vehicle::Vehicle(unsigned int l, unsigned int w, unsigned int state_code) :
    length(l),
    width(w),
    orientation(0),
    rear_center(0,0),
    rear_left(0,0),
    rear_right(0,0),
    front_center(0,0),
    front_left(0,0),
    front_right(0,0)
{
    std::vector<double> state = Vehicle::decode_vehicle(state_code);
    orientation = state[0];
    rear_center = Coordinate(state[1], state[2]);
    discretize_coords();
}


/**
 * A vehicle copied from another.
 * 
 * @param v1 the vehicle to copy
 */
Vehicle::Vehicle(const Vehicle& v1) :
    length(v1.get_length()),
    width(v1.get_width()),
    orientation(v1.get_orientation()),
    rear_center(v1.get_rear_center()),
    rear_left(v1.get_rear_left()),
    rear_right(v1.get_rear_right()),
    front_center(v1.get_front_center()),
    front_left(v1.get_front_left()),
    front_right(v1.get_front_right()) 
{}


/**
 * Check if the vehicle is colliding with any object in the map.
 * 
 * @param m the map
 * @return true if colliding, false otherwise
 */
bool Vehicle::verify_collision(const Map& m) const {
    std::vector<Coordinate> coords = {rear_left, rear_right, front_right, front_left};
    Polygon car(coords);
    for (auto ob : m.obstacles)
        if (check_collision(ob, car))
            return true;
    return false;
}


/**
 * Computer the rotation center coordinate of a maneuver.
 * 
 * @param vehicle the vehicle rear axis coordinate
 * @param orientation the vehicle orientation
 * @param r the turning radius of the maneuver
 * @param s the spin of the maneuver
 * @param v the verse of the maneuver
 * @return the Coordinate of the rotation center
 */
Coordinate compute_rotation_center(Coordinate vehicle, double orientation, TurningRadius r, Spin s, Verse v) {
    assert (s == CLOCKWISE || s == COUNTERCLOCKWISE);

    Direction head = Direction::from_angle(orientation);
    if ((s == CLOCKWISE && v == FORWARD) || (s == COUNTERCLOCKWISE && v == BACKWARDS))
        return vehicle + (head.get_right_perp() * (double)r);
    else
        return vehicle + (head.get_left_perp() * (double)r);
}


/**
 * Check if the trajectory of the vehicle performing a maneuver generates a crash.
 * 
 * @param start_vehicle the vehicle before moving
 * @param final_vehicle the vehicle after moving
 * @param rot_center the rotation center
 * @param verse the verse of the maneuver
 * @param spin the spin of the maneuver
 * @param map the map
 * @return true if a collision occurs, false otherwise
 */
bool trajectory_collides(const Vehicle& start_vehicle, const Vehicle& final_vehicle,
                         Coordinate rot_center, Verse verse, Spin spin, const Map& map) {
    if ((spin == CLOCKWISE) == (verse == FORWARD)) {
        Coordinate RR_start = start_vehicle.get_rear_right(),
                   RR_final = final_vehicle.get_rear_right(),
                   FL_start = start_vehicle.get_front_left(),
                   FL_final = final_vehicle.get_front_left();
        double RR_start_angle = (RR_start - rot_center).get_angle(),
               RR_final_angle = (RR_final - rot_center).get_angle(),
               RR_radius = std::min((RR_start - rot_center).get_modulus(), (RR_final - rot_center).get_modulus()),
               FL_start_angle = (start_vehicle.get_front_left() - rot_center).get_angle(),
               FL_final_angle = (final_vehicle.get_front_left() - rot_center).get_angle(),
               FL_radius = std::max((FL_start - rot_center).get_modulus(), (FL_final - rot_center).get_modulus());
        if (verse == FORWARD) {     // Arc angles must be specified counterclockwisely
            std::swap(RR_start_angle, RR_final_angle);
            std::swap(FL_start_angle, FL_final_angle);
        }
        Arc RR_arc(rot_center, RR_radius, RR_start_angle, RR_final_angle);
        Arc FL_arc(rot_center, FL_radius, FL_start_angle, FL_final_angle);
        
        if (map.collides_with_obstacles(RR_arc) || map.collides_with_obstacles(FL_arc))
            return true;
    } else {
        Coordinate RL_start = start_vehicle.get_rear_left(),
                   RL_final = final_vehicle.get_rear_left(),
                   FR_start = start_vehicle.get_front_right(),
                   FR_final = final_vehicle.get_front_right();
        double RL_start_angle = (RL_start - rot_center).get_angle(),
               RL_final_angle = (RL_final - rot_center).get_angle(),
               RL_radius = std::min((RL_start - rot_center).get_modulus(), (RL_final - rot_center).get_modulus()),
               FR_start_angle = (FR_start - rot_center).get_angle(),
               FR_final_angle = (FR_final - rot_center).get_angle(),
               FR_radius = std::max((FR_start - rot_center).get_modulus(), (FR_final - rot_center).get_modulus());
        if (verse == BACKWARDS) {     // Arc angles must be specified counterclockwisely
            std::swap(RL_start_angle, RL_final_angle);
            std::swap(FR_start_angle, FR_final_angle);
        }
        Arc RL_arc(rot_center, RL_radius, RL_start_angle, RL_final_angle);
        Arc FR_arc(rot_center, FR_radius, FR_start_angle, FR_final_angle);
        
        if (map.collides_with_obstacles(RL_arc) || map.collides_with_obstacles(FR_arc))
            return true;
    }
    return false;
}


/**
 * Change the position and orientation of the vehicle.
 * 
 * @param map the map
 * @param pos the new position
 * @param angle the new orientation
 * @return 0 if the new state is fine, 1 if collision detected, 2 if boundary
 *      violation occurs, 3 if the orientation is invalid
 */
unsigned int Vehicle::reposition(Map& map, Coordinate pos, double angle) {
    
    // Update the position and angle
    rear_center = pos;
    orientation = wrap_angle(angle);
    
    // Check if the new state is encodable (position and angle within boundaries)
    if (orientation >= pi)
        return 3;
    if (!map.is_within_boundaries(rear_center)) // Note: only RC is needed for encoding
        return 2;
    
    // Adjust the coordinates (always works thanks to the previous checks)
    discretize_coords();
    
    // Verify if the whole vehicle (not only RC!) is still inside the map
    Polygon p = this->to_polygon();
    if (!map.is_within_boundaries(p))
        return 2;

    // Check if the vehicle collided with any obstacle
    if (map.collides_with_obstacles(p))
        return 1;

    return 0;
}


/**
 * Move the vehicle according a specific maneuver. The final position is then
 * discretized to comply with the closest state.
 * 
 * @param map the map
 * @param mnv the maneuver
 * @return 0 if the new state is fine, 1 if collision detected, 2 if boundary
 *      violation occurs, 3 if the new orientation is invalid
 */
unsigned int Vehicle::move(Map& map, Maneuver& mnv) {
    
    Coordinate new_pos(0,0);    // bogus initialization
    double new_angle;
    unsigned int ret;
    
    // Save the current state of the vehicle (ghost)
    Vehicle ghost(*this);

    // Compute the new state
    Verse verse = mnv.get_verse();
    Spin spin = mnv.get_spin();
    Displacement arc_length = mnv.get_displacement();
   
    switch(spin) {
        case STRAIGHT:
            // Compute the new position and angle
            new_pos = rear_center + verse * Direction::from_angle(orientation) * arc_length;
            new_angle = orientation;
            
            // Move the vehicle there, checking the validity of the state
            if ((ret = reposition(map, new_pos, new_angle)))
                return ret;
            break;
        case CLOCKWISE:
        case COUNTERCLOCKWISE:
            // Compute the new position and angle
            TurningRadius radius = mnv.get_turning_radius();
            Coordinate rot_center = compute_rotation_center(rear_center, orientation, radius, spin, verse);
            double delta_angle;
            if (spin == CLOCKWISE)
                delta_angle = - double(arc_length) / radius;
            else
                delta_angle = double(arc_length) / radius;
            new_pos = rear_center.rotate(rot_center, delta_angle);
            new_angle = wrap_angle(orientation + delta_angle);
            
            // Move the vehicle to the new position, checking the validity of the state
            if ((ret = reposition(map, new_pos, new_angle)))
                return ret;
            
            // Check for trajectory arcs collisions
            if (trajectory_collides(ghost, *this, rot_center, verse, spin, map))
                return 1;
    }
    return 0;
}


/**
 * Generate a Vehicle with random position and orientation.
 * 
 * @return a random vehicle
 */
Vehicle Vehicle::random_vehicle() {
    unsigned int seed = rand() % (HREF_POINTS * VREF_POINTS * ANGLE_REFS);
    std::vector<double> state = decode_vehicle(seed);
    return Vehicle(CAR_LENGTH, CAR_WIDTH, Coordinate(state[1], state[2]), state[0]);
}


/**
 * Get the Polygon associated to the body of the vehicle.
 * 
 * @return the polygon of the car
 */
Polygon Vehicle::to_polygon() const {
    return Polygon(this->get_vertices1());
}


/**
 * Decode the vehicle from its state
 * 
 * @param code the state code
 * @return a vector of 3 elements, storing the orientation, x and y, respectively
 */
std::vector<double> Vehicle::decode_vehicle(unsigned int code) {
    std::vector<double> ret(3);
    /* A state actually corresponds to a cluster of points. Adding 0.5 makes sure
     * we return the one in the middle. */
    ret[0] = ((code % ANGLE_REFS) + 0.5) * (pi / ANGLE_REFS);           // orientation
    ret[2] = (((code / ANGLE_REFS) % VREF_POINTS) + 0.5) * SPACE_UNIT;  // y
    ret[1] = (((code / ANGLE_REFS / VREF_POINTS)) + 0.5) * SPACE_UNIT;  // x
    return ret;
}


/**
 * Encode the vehicle into a code fully identifying its state.
 * 
 * @param orientation the current vehicle orientation
 * @param x the x coordinate of rear axis center
 * @param y the y coordinate of rear axis center
 * @return the state code if encodable, assert failure otherwise
 */
unsigned int Vehicle::encode_vehicle(double orientation, double x, double y) {
    
    unsigned int orientation_box = floor(ANGLE_REFS * orientation / pi);
    unsigned int x_box = floor(x / SPACE_UNIT);
    unsigned int y_box = floor(y / SPACE_UNIT);
    
    assert(orientation_box < ANGLE_REFS && 
        "Can't encode the vehicle (orientation out of range). Make sure to check"
        "the consistency of arguments before calling this function.");
    assert(x_box < HREF_POINTS && 
        "Can't encode the vehicle (x out of range). Make sure to check"
        "the consistency of arguments before calling this function.");
    assert(y_box < VREF_POINTS &&
        "Can't encode the vehicle (y out of range). Make sure to check"
        "the consistency of arguments before calling this function.");
    
    return orientation_box + ANGLE_REFS * y_box + ANGLE_REFS * VREF_POINTS * x_box;
}


/**
 * Encode the vehicle into a code fully identifying its state.
 * 
 * @return the code of the state
 */
unsigned int Vehicle::encode() {
    return encode_vehicle(orientation, rear_center.x, rear_center.y);
}


/**
 * Assignment operator for Vehicle.
 * 
 * @param v1 the vehicle to copy
 * @return the copied vehicle
 */
Vehicle& Vehicle::operator=(const Vehicle& v1) {
    length = v1.get_length();
    width = v1.get_width();
    orientation = v1.get_orientation();
    rear_center = v1.get_rear_center();
    compute_secondary_coords();
    return *this;
}


/**
 * Ostream operator for Vehicle.
 * 
 * @param os the ostream
 * @param v the vehicle
 * @return the ostream
 */
std::ostream& operator<<(std::ostream& os, const Vehicle& v) {
	os << "Position: " << v.rear_center << "\nOrientation: " << v.orientation;
	return os;
}