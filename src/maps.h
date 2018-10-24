#ifndef MAPS_H
#define MAPS_H

#include "map.h"

/******************************************************************************/
/*          SIMPLE PARKING MAP: a typical parallel parking scenario           */
/******************************************************************************/

class SimpleParkingMap : public Map {
    private:
        void initialize();
    public:
        SimpleParkingMap();
};


/******************************************************************************/
/*             TWISTED PARKING MAP: a zig-zag parking scenario                */
/******************************************************************************/

class TwistedParkingMap : public Map {
    private:
        void initialize();
    public:
        TwistedParkingMap();
};


#endif /* MAPS_H */

