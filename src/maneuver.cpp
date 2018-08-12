#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include "maneuver.h"

using namespace std;

Verse Maneuver::get_verse() const {
    return verse;
}

Spin Maneuver::get_spin() const {
    return spin;
}

TurningRadius Maneuver::get_turning_radius() const {
    return turning_radius;
}

Displacement Maneuver::get_displacement() const {
    return displacement;
}

unsigned int Maneuver::encode_maneuver() const {
    uint8_t dir_code = NUM_RADI + spin*(turning_radius/MIN_RADI);
    return (verse > 0 ? NUM_DISP * (2 * NUM_RADI + 1) : 0) +
            NUM_DISP * dir_code +
            (displacement/MIN_DISP - 1);
}

void Maneuver::decode_maneuver(unsigned int code) {
    displacement = static_cast<Displacement>(MIN_DISP * (1 + code % 3));
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
    verse = static_cast<Verse>(code >=  NUM_DISP * (2 * NUM_RADI + 1) ? 1 : -1);
}

ostream& operator<<(ostream& os, const Maneuver& m) {
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
    os << m.displacement << "cm" << endl;
    return os;
}

Maneuver Maneuver::random_maneuver() {
	return Maneuver(rand() % 30);
}

Maneuver::Maneuver(unsigned int code) {
	if (code > 2 * NUM_DISP * (2 * NUM_RADI + 1)){
		throw invalid_argument("Invalid argument (maneuver code)");
	}
	decode_maneuver(code);
}

