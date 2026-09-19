#pragma once
#include<string>

struct WindData {
    double speedKnots = 0.0;
    double directionDegrees = 0.0;
    bool success = false;
};

class WeatherClient {
public:
    static WindData getWindAtCoordinates(double lat, double lon);
};