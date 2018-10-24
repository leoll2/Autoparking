#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include "maneuver.h"


/******************************************************************************/
/*              MANEUVER: an action performed by a vehicle                    */
/******************************************************************************/

/**
 * Get the verse associated to the maneuver.
 * 
 * @return the verse
 */
Verse Maneuver::get_verse() const {
    return verse;
}


/**
 * Get the turning direction (spin) associated to the maneuver.
 * 
 * @return the spin
 */
Spin Maneuver::get_spin() const {
    return spin;
}


/**
 * Get the turning radius associated to the maneuver.
 * 
 * @return the turning radius
 */
TurningRadius Maneuver::get_turning_radius() const {
    return turning_radius;
}


/**
 * Get the distance (displacement) traveled by the car when performing this maneuver.
 * 
 * @return the displacement
 */
Displacement Maneuver::get_displacement() const {
    return displacement;
}


/**
 * Convert the maneuver in a unique code.
 * 
 * @return the encoded maneuver
 */
unsigned int Maneuver::encode_maneuver() const {
    
    uint8_t dir_code = NUM_RADI + spin*(turning_radius/MIN_RADI);
    return (verse > 0 ? NUM_DISP * (2 * NUM_RADI + 1) : 0) +
            NUM_DISP * dir_code +
            (displacement/MIN_DISP - 1);
}


/**
 * Decode a maneuver from its code. Initializes the attributes with proper values.
 * 
 * @param code the code of the maneuver
 */
void Maneuver::decode_maneuver(unsigned int code) {
    
    // Compute the displacement
    displacement = static_cast<Displacement>(MIN_DISP * (1 + code % 3));
    
    // Compute the spin and turning radius
    uint8_t dir_code = (code / NUM_DISP) % (2 * NUM_RADI + 1);
    if (dir_code < NUM_RADI) {
        spin = static_cast<Spin>(-1);
        turning_radius = static_cast<TurningRadius>(MIN_RADI*(NUM_RADI - dir_code));
    } else if (dir_code == NUM_RADI) {
        spin = static_cast<Spin>(0);
    } else {
        spin = static_cast<Spin>(1);
        turning_radius = static_cast<TurningRadius>(MIN_RADI*(dir_code - NUM_RADI));
    }
    
    // Compute the verse
    verse = static_cast<Verse>(code >=  NUM_DISP * (2 * NUM_RADI + 1) ? 1 : -1);
}


/**
 * Overload of the ostream operator for Maneuver objects.
 * 
 * @param os the ostream object
 * @param m the maneuver to be printed
 * @return the ostream object
 */
std::ostream& operator<<(std::ostream& os, const Maneuver& m) {
    os << (m.verse > 0 ? "fore " : "back ");
    switch (m.spin) {
        case -1:
            os << "counterclockwise " << m.turning_radius << "cm ";
            break;
        case 0:
            os << "straight ";
            break;
        case 1:
            os << "clockwise " << m.turning_radius << "cm "; 
    }
    os << m.displacement << "cm" << std::endl;
    return os;
}


/**
 * Generate a random maneuver.
 * 
 * @return a random Maneuver
 */
Maneuver Maneuver::random_maneuver() {
    return Maneuver(rand() % 30);
}


/**
 * A Maneuver represent an action that can be performed by the vehicle.
 * 
 * @param code the code associated to the maneuver
 */
Maneuver::Maneuver(unsigned int code) {
    
    if (code > 2 * NUM_DISP * (2 * NUM_RADI + 1)){
        throw std::invalid_argument("Invalid argument (maneuver code)");
    }
    decode_maneuver(code);
}

