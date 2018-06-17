#include <iostream>
#include "maneuver.h"

using namespace std;

int main() {
	for (unsigned int i = 0; i < 30; ++i) {
		cout << i << ", ";
		Maneuver man(i);
		cout << man.encode_maneuver() << ": ";
		cout << man << endl;
	}
	
}
