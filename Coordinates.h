#ifndef COORDINATES_H
#define COORDINATES_H

struct Coordinates {

    // Position
    double latitude; // A change in latitude is sine of heading
    double longitude; // A change in longitude is cosine of heading

    // 3D & Environmental Ground Metrics
    double verticalspeed;     // in feet per minute (FPM)
    double groundspeed;       // In knots (calculated speed over ground)
    double velocityN;         // North/South ground velocity component
    double velocityE;         // East/West ground velocity component

};
#endif
