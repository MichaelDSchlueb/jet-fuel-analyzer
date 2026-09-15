#pragma once
#include<string>

struct WindData {
    double speedKnots = 0.0;
    double directionDegrees = 0.0;
};

class WeatherClient {
public:
    static WindData getWindAtCoordiantes(double lat, double lon);
};