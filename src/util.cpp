#include "util.h"

using namespace std;

Direction Direction::from_angle(double angle) {
	return Direction(cos(angle), sin(angle));
}

double Direction::get_angle() {
	double angle = atan2(y, x);
	if (angle < 0)
		return angle + 2 * pi;
	else
		return angle;
}

double Direction::get_modulus() {
	return sqrt(x * x + y * y);
}

Direction Direction::get_right_perp() {
	double modulus = get_modulus();
	return (Direction(y, -x) * (1/modulus));
}

Direction Direction::get_left_perp() {
	double modulus = get_modulus();
	return (Direction(-y, x) * (1/modulus));
}

Direction Direction::operator+(const Direction& d1) const {
	return Direction(x + d1.x, y + d1.y);
}

Direction Direction::operator-(const Direction& d1) const {
	return Direction(x - d1.x, y - d1.y);
}


double Direction::operator*(const Direction& d1) const {
	return x * d1.x + y * d1.y;
}

Direction operator*(const Direction& d1, const double a) {
	return Direction(d1.x * a, d1.y * a);
}

Direction operator*(const double a, const Direction& d1) {
	return d1 * a;
}

Direction::Direction(double x, double y) :
	x(x), y(y) {};

std::ostream& operator<<(std::ostream& os, const Direction& d) {
	os << "(" << d.x << ", " << d.y << ")";
	return os;
}