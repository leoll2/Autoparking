#include "util.h"

using namespace std;

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

double Direction::get_angle() {
	double angle = atan2(y, x);
	if (angle < 0)
		return angle + 2 * pi;
	else
		return angle;
}

Direction::Direction(double x, double y) :
	x(x), y(y) {};

/*double dot(const Direction& a, const Direction& b) {
	return a.x * b.x + a.y * b.y;
};*/