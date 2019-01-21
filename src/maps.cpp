#include "maps.h"

/******************************************************************************/
/*          SIMPLE PARKING MAP: a typical parallel parking scenario           */
/******************************************************************************/

/**
 * Initialize the map adding obstacles.
 */
void SimpleParkingMap::initialize() {

    // Add first obstacle
    std::vector<Coordinate> ob1_coords = {	//must be counterclockwise
            {HREF_POINTS*7/12*SPACE_UNIT,   VREF_POINTS*3/4*SPACE_UNIT},
            {HREF_POINTS*1*SPACE_UNIT,      VREF_POINTS*3/4*SPACE_UNIT},
            {HREF_POINTS*1*SPACE_UNIT,      VREF_POINTS*1*SPACE_UNIT},
            {HREF_POINTS*7/12*SPACE_UNIT,   VREF_POINTS*1*SPACE_UNIT}};
    Polygon ob1(ob1_coords);
    this->add_obstacle(ob1);

    // Add second obstacle
    std::vector<Coordinate> ob2_coords = {	//must be counterclockwise
            {HREF_POINTS*7/12*SPACE_UNIT,   VREF_POINTS*0*SPACE_UNIT},
            {HREF_POINTS*1*SPACE_UNIT,      VREF_POINTS*0*SPACE_UNIT},
            {HREF_POINTS*1*SPACE_UNIT,      VREF_POINTS*1/4*SPACE_UNIT},
            {HREF_POINTS*7/12*SPACE_UNIT,   VREF_POINTS*1/4*SPACE_UNIT}};
    Polygon ob2(ob2_coords);
    this->add_obstacle(ob2);
}

/**
 * A typical parallel parking map.
 */
SimpleParkingMap::SimpleParkingMap() : 
        Map(HREF_POINTS, VREF_POINTS, Coordinate(HREF_POINTS*3/4*SPACE_UNIT, VREF_POINTS*1/3*SPACE_UNIT)) {
    initialize();
}


unsigned int SimpleParkingMap::get_id() {
    return 0;
}


/******************************************************************************/
/*             TWISTED PARKING MAP: a zig-zag parking scenario                */
/******************************************************************************/

/**
 * Initialize the map adding obstacles.
 */
void TwistedParkingMap::initialize() {
    
    /* Add the first obstacle */
    std::vector<Coordinate> ob1_coords = {	//must be counterclockwise
            {HREF_POINTS*0*SPACE_UNIT,      VREF_POINTS*2/3*SPACE_UNIT},
            {HREF_POINTS*5/12*SPACE_UNIT,   VREF_POINTS*2/3*SPACE_UNIT},
            {HREF_POINTS*5/12*SPACE_UNIT,   VREF_POINTS*1*SPACE_UNIT},
            {HREF_POINTS*0*SPACE_UNIT,      VREF_POINTS*1*SPACE_UNIT}};
    Polygon ob1(ob1_coords);
    this->add_obstacle(ob1);

    // Add the second obstacle
    std::vector<Coordinate> ob2_coords = {	//must be counterclockwise
            {HREF_POINTS*7/12*SPACE_UNIT,   VREF_POINTS*0*SPACE_UNIT},
            {HREF_POINTS*1*SPACE_UNIT,      VREF_POINTS*0*SPACE_UNIT},
            {HREF_POINTS*1*SPACE_UNIT,      VREF_POINTS*1/4*SPACE_UNIT},
            {HREF_POINTS*7/12*SPACE_UNIT,   VREF_POINTS*1/4*SPACE_UNIT}};
    Polygon ob2(ob2_coords);
    this->add_obstacle(ob2);
}


/**
 * A zig-zag parking map.
 */
TwistedParkingMap::TwistedParkingMap() : 
        Map(HREF_POINTS, VREF_POINTS, Coordinate(HREF_POINTS*2/3*SPACE_UNIT, VREF_POINTS*7/12*SPACE_UNIT)) {
    initialize();
}


unsigned int TwistedParkingMap::get_id() {
    return 1;
}