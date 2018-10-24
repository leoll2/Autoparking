#include <iostream>
#include "maneuver.h"

using namespace std;

int main() {
    
    // Decode and re-encode maneuver and verify that the two codes are equal.
    for (unsigned int i = 0; i < 30; ++i) {
        cout << i << ", ";
        Maneuver man(i);
        cout << man.encode_maneuver() << ": ";
        cout << man << endl;
    }
    return 0;
}
