#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "field_params.h"
#include "vehicle.h"
#include "vehicle_params.h"

using namespace std;

int Vehicle::get_length() const {
    return length;
}

int Vehicle::get_width() const {
    return width;
}

double Vehicle::get_orientation() const {
    return orientation;
}

Coordinate Vehicle::get_rear_center() const {
    return rear_center;
}

Coordinate Vehicle::get_rear_left() const {
    return rear_left;
}

Coordinate Vehicle::get_rear_right() const {
    return rear_right;
}

Coordinate Vehicle::get_front_center() const {
    return front_center;
}

Coordinate Vehicle::get_front_left() const {
    return front_left;
}

Coordinate Vehicle::get_front_right() const {
    return front_right;
}

std::vector<Coordinate> Vehicle::get_vertices1() const {
    std::vector<Coordinate> ret;
    ret.push_back(rear_left);
    ret.push_back(rear_right);
    ret.push_back(front_right);
    ret.push_back(front_left);
    return ret;
}

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

void Vehicle::compute_secondary_coords() {
    rear_left  =    Coordinate((rear_center.x - (width/2 * sin(orientation))) , (rear_center.y + (width/2 * cos(orientation))));
    rear_right =    Coordinate((rear_center.x + (width/2 * sin(orientation))) , (rear_center.y - (width/2 * cos(orientation))));
    front_center =  Coordinate((rear_center.x + (length  * cos(orientation))) , (rear_center.y + (length  * sin(orientation))));
    front_left =    Coordinate((front_center.x - (width/2 * sin(orientation))) , (front_center.y + (width/2 * cos(orientation))));
    front_right =   Coordinate((front_center.x + (width/2 * sin(orientation))) , (front_center.y - (width/2 * cos(orientation))));
}

void Vehicle::discretize_coords() {
    std::vector<double> state = decode_vehicle(this->encode());
    rear_center = Coordinate(state[1], state[2]);
    orientation = state[0];
    compute_secondary_coords();
}

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
        discretize_coords();    // compute_secondary_coords() is called within discretize_coords()
    else
        compute_secondary_coords();
}

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

bool Vehicle::verify_collision(const Map& m) const {
    std::vector<Coordinate> coords = {rear_left, rear_right, front_right, front_left};
    Polygon car(coords);
    for (auto ob : m.obstacles)
        if (check_collision(ob, car))
            return true;
    return false;
}

Coordinate compute_rotation_center(Coordinate vehicle, double orientation, TurningRadius r, Spin s, Verse v) {
    assert (s == CLOCKWISE || s == COUNTERCLOCKWISE);

    Direction head = Direction::from_angle(orientation);
    if ((s == CLOCKWISE && v == FORWARD) || (s == COUNTERCLOCKWISE && v == BACKWARDS))
        return vehicle + (head.get_right_perp() * (double)r);
    else
        return vehicle + (head.get_left_perp() * (double)r);
}

unsigned int Vehicle::reposition(Map& map, Coordinate pos, double angle) {
    // Update position and angle
    rear_center = pos;
    orientation = wrap_angle(angle);
    
    // Check if the new state is encodable (position and angle within boundaries)
    if (orientation >= pi)
        return 3;
    if (!map.is_within_boundaries(rear_center)) // Note: only RC is needed for encoding
        return 2;
    
    discretize_coords();    // this is legit thanks to the previous checks
    
    // Verify if the whole vehicle (not only RC!) is still inside the map
    Polygon p = this->to_polygon();
    if (!map.is_within_boundaries(p))
        return 2;

    // Check if the vehicle collided with any obstacle
    if (map.collides_with_obstacles(p))
        return 1;

    return 0;
}

unsigned int Vehicle::move(Map& map, Maneuver& mnv) {
    // Save the current state of the vehicle (ghost)
    Vehicle ghost(*this);

    // Compute the new state
    Verse verse = mnv.get_verse();
    Spin spin = mnv.get_spin();
    Displacement arc_length = mnv.get_displacement();
    Coordinate new_pos(0,0);    // bogus initialization
    double new_angle;
    switch(spin) {
        case STRAIGHT:
            new_pos = rear_center + verse * Direction::from_angle(orientation) * arc_length;
            new_angle = orientation;
            break;
        case CLOCKWISE:
        case COUNTERCLOCKWISE:
            TurningRadius radius = mnv.get_turning_radius();
            Coordinate rot_center = compute_rotation_center(rear_center, orientation, radius, spin, verse);
            double delta_angle;
            if (spin == CLOCKWISE)
                delta_angle = - double(arc_length) / radius;
            else
                delta_angle = double(arc_length) / radius;
            new_pos = rear_center.rotate(rot_center, delta_angle);
            new_angle = wrap_angle(orientation + delta_angle);
    }
    // Move the vehicle to the new position, checking the validity of the state
    unsigned int ret = reposition(map, new_pos, new_angle);
    if (ret)
        return ret;
    
    // TO BE IMPLEMENTED: VERIFICA COLLISIONE ARCHI!!!

    return 0;
}

Vehicle Vehicle::random_vehicle() {
    unsigned int seed = rand() % (HREF_POINTS * VREF_POINTS * ANGLE_REFS);
    std::vector<double> state = decode_vehicle(seed);
    return Vehicle(CAR_LENGTH, CAR_WIDTH, Coordinate(state[1], state[2]), state[0]);
}

Polygon Vehicle::to_polygon() const {
    return Polygon(this->get_vertices1());
}

std::vector<double> Vehicle::decode_vehicle(unsigned int code) {
    std::vector<double> ret(3);
    /* A state actually corresponds to a cluster of points. Adding 0.5 makes sure
     * we return the one in the middle. */
    ret[0] = ((code % ANGLE_REFS) + 0.5) * (pi / ANGLE_REFS);           // orientation
    ret[2] = (((code / ANGLE_REFS) % VREF_POINTS) + 0.5) * SPACE_UNIT;  // y
    ret[1] = (((code / ANGLE_REFS / VREF_POINTS)) + 0.5) * SPACE_UNIT;  // x
    return ret;
}

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

unsigned int Vehicle::encode() {
    return encode_vehicle(orientation, rear_center.x, rear_center.y);
}

Vehicle& Vehicle::operator=(const Vehicle& v1) {
    length = v1.get_length();
    width = v1.get_width();
    orientation = v1.get_orientation();
    rear_center = v1.get_rear_center();
    compute_secondary_coords();
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& v) {
	os << "Position: " << v.rear_center << "\nOrientation: " << v.orientation;
	return os;
}