#include "WeatherClient.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

WindData WeatherClient::getWindAtCoordiantes(double lat, double lon) {
    WindData wind;
    std::string url = "https://ap.open-meteo.com/v1/forecast?latitude="
                        + std::to_string(lat)
                        + "&longitude=" + std::to_string(lon)
                        + "&current=wind_speed_10m,wind_direction_10m&wind_speed_unit=kn";
    cpr::Response r = cpr::Get(cpr::Url{url}, cpr::Timeout{3000});

    if (r.status_code == 200) {
        try {
            auto daa = json::parse(r.text);
            wind.speedKnots = daa["current_weather"]["wind_speed_10m"].get<double>();
            wind.directionDegrees = daa["current_weather"]["wind_direction_10m"].get<double>();
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Error fetching weather data: HTTP " << r.status_code << std::endl;
    }

    return wind;
}