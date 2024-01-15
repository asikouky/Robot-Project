#include "gps.h"

const uint8_t MAX_WEIGHT = 100;
const uint8_t MAX_COLUMN = 7;
struct Link {
    uint8_t node1;
    uint8_t node2;
    uint8_t weight = 1;
};

Link links[35] = {
    {1,2},
    {2,3},
    {3,4},
    {5,6},
    {6,7,2},
    {1,8},
    {3,10},
    {4,11},
    {6,13,2},
    {7,14},
    {8,9,5},
    {9,10},
    {11,12,2},
    {12,13},
    {13,14},
    {9,16},
    {10,17,5},
    {12,19,5},
    {13,20},
    {14,21},
    {15,16,2},
    {16,17},
    {17,18},
    {18,19,5},
    {15,22,5},
    {17,24},
    {18,25},
    {19,26,5},
    {20,27},    
    {21,28},
    {22,23},
    {23,24},
    {25,26,2},
    {26,27,5},
    {27,28}
};

uint8_t coordsToPoint(uint8_t xPos, uint8_t yPos){
    return MAX_COLUMN * (xPos-1) + yPos;
}


GPS::GPS(){}

void GPS::placePole(uint8_t xPos, uint8_t yPos, Compass orientation){
    switch(orientation){
        case Compass::NORTH:
            xPos--;
            break;
        case Compass::EAST:
            yPos++;
            break;
        case Compass::SOUTH:
            xPos++;
            break;
        case Compass::WEST:
            yPos--;
            break;
    }
    pole_ = coordsToPoint(xPos, yPos);
}

uint8_t GPS::findWeight(uint8_t node1,uint8_t node2) {
        if (node1 == pole_ || node2 == pole_)
            return MAX_WEIGHT;

        if (node1 > node2){
            uint8_t tmp = node1;
            node1 = node2;
            node2 = tmp;
        }

        for (Link link : links){
            if (node1 == link.node1 && node2 == link.node2)
                return link.weight;
        }
        return MAX_WEIGHT;
    }

Compass GPS::nextDirection(uint8_t xPos, uint8_t yPos) {
    uint8_t currentNode = coordsToPoint(xPos, yPos);
    uint8_t i = 0;
    while (route[i] != currentNode && i < 28)
        i++;
    //si la position du robot dans le trajet est inconnu
    if (i == 28) {
        return Compass::SOUTH;
    }
    uint8_t xNextPos = (route[i+1] - 1) / MAX_COLUMN + 1;
    uint8_t yNextPos = (route[i+1] - 1) % MAX_COLUMN + 1;
    if (xNextPos == xPos) {
        if (yNextPos == yPos + 1)
            return Compass::EAST;
        return Compass::WEST;
    }
    if (xNextPos == xPos + 1)
        return Compass::SOUTH;
    return Compass::NORTH;
}

void GPS::setItinerary(uint8_t xPos, uint8_t yPos, uint8_t xEnd, uint8_t yEnd) {
    source_ = coordsToPoint(xPos, yPos);
    destination_ = coordsToPoint(xEnd, yEnd);
    uint8_t closestFromStart[28];

    dijkstra(source_, destination_, closestFromStart);
    uint8_t size = 0;
    uint8_t currentNode = destination_;
    while (closestFromStart[currentNode - 1] != 0) {
        route[28 - ++size] = currentNode;
        currentNode = closestFromStart[currentNode - 1];
    }
    //ajout de la position initiale
    route[28 - ++size] = source_;

    //mets les valeurs au debut du array
    for (uint8_t i = 0; i < size; i++) {
        route[i] = route[28 - size + i];
    }
    pole_ = 0;

}

void GPS::dijkstra(uint8_t source, uint8_t destination, uint8_t closestFromStart[28]) {
    uint8_t lowestDistance[28];

    for (uint8_t i = 0; i < 28; i++) {
        lowestDistance[i] = MAX_WEIGHT;
    }

    uint8_t currentNode = source;
    closestFromStart[currentNode - 1] = 0;
    lowestDistance[currentNode - 1] = 0;
    
    while (lowestDistance[destination - 1] != 0) {

        uint8_t currentWeight = lowestDistance[currentNode - 1];
        uint8_t nextWeight;

        for (uint8_t node = 1; node < 29; node++) {
            if (currentNode == node) {
                //un 0 indique que le noeud est deja explore
                lowestDistance[currentNode - 1] = 0;
                continue;
            }

            nextWeight = findWeight(currentNode, node) + currentWeight;
            if (nextWeight < lowestDistance[node - 1]) {
                lowestDistance[node - 1] = nextWeight;
                closestFromStart[node - 1] = currentNode;
            }
        }

        uint8_t min = MAX_WEIGHT;
        for (uint8_t node = 1; node < 29; node++) {
            if (lowestDistance[node - 1] < min && lowestDistance[node - 1] != 0) {
                currentNode = node;
                min = lowestDistance[node - 1];
            }
        }
    }
}
