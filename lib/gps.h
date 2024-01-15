/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef GPS_
#define GPS_
#include <avr/io.h> 

/*
Description du programme:


Identifications matérielles: 
    - 
    -

*/

enum Compass {NORTH, EAST, SOUTH, WEST}; 

class GPS {
public:
    GPS();
    void setItinerary(uint8_t xPos, uint8_t yPos, uint8_t xEnd, uint8_t yEnd);
    void placePole(uint8_t xPos, uint8_t yPos, Compass orientation);
    Compass nextDirection(uint8_t xPos, uint8_t yPos);

private:
    uint8_t source_;
    uint8_t destination_;
    uint8_t route[28];
    uint8_t pole_ = 0;
    void dijkstra(uint8_t source, uint8_t destination, uint8_t closestFromStart[28]);
    uint8_t findWeight(uint8_t node1, uint8_t node2);
    
};
#endif