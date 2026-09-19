#include "WeatherClient.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#else
#include <cpr/cpr.h>
#endif

using json = nlohmann::json;

WindData WeatherClient::getWindAtCoordinates(double lat, double lon) {
    WindData wind;
    std::string responseText;

#ifdef _WIN32
    // Windows Native HTTP Request via WinINet
    HINTERNET hInternet = ::InternetOpenA("QuiltEngine/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet) {
        std::string url = "http://api.open-meteo.com/v1/forecast?latitude=" 
                          + std::to_string(lat) + "&longitude=" + std::to_string(lon) 
                          + "&current=wind_speed_10m,wind_direction_10m&wind_speed_unit=kn";

        HINTERNET hUrl = ::InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hUrl) {
            char buffer[1024];
            DWORD bytesRead = 0;
            while (::InternetReadFile(hUrl, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                responseText += buffer;
            }
            ::InternetCloseHandle(hUrl);
        }
        ::InternetCloseHandle(hInternet);
    }
#else
    // Linux HTTP Request via CPR
    std::string url = "https://api.open-meteo.com/v1/forecast?latitude=" 
                      + std::to_string(lat) + "&longitude=" + std::to_string(lon) 
                      + "&current=wind_speed_10m,wind_direction_10m&wind_speed_unit=kn";
    cpr::Response r = cpr::Get(cpr::Url{url}, cpr::Timeout{3000});
    if (r.status_code == 200) {
        responseText = r.text;
    }
#endif

    if (!responseText.empty()) {
        try {
            auto data = json::parse(responseText);
            wind.speedKnots = data["current"]["wind_speed_10m"].get<double>();
            wind.directionDegrees = data["current"]["wind_direction_10m"].get<double>();
            wind.success = true;
        } catch (const std::exception& e) {
            std::cerr << "[WeatherClient] JSON Parsing Error: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "[WeatherClient] HTTP Request failed or returned empty payload." << std::endl;
    }

    return wind;
}