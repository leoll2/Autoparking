#ifndef MAPS_H
#define MAPS_H

#include "map.h"

class SimpleParkingMap : public Map {
    private:
        void initialize();
    public:
        SimpleParkingMap();
};

class TwistedParkingMap : public Map {
    private:
        void initialize();
    public:
        TwistedParkingMap();
};


#endif /* MAPS_H */

