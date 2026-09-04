#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cstdint>
#include "Coordinates.h"
#include "EngineInstruments.h"
#include "FuelSystem.h"
#include <iostream>
#include <string>
#include <chrono>

enum struct FlightPhase {
     GATE,
	PUSHBACK,
	TAXI,
	TAKEOFF_ROLL,
	CLIMB
};

struct Aircraft {

private:
     std::string tailNumber; // The Physical Plane
     uint64_t timestamp;
     Coordinates position;
     FuelSystem fuel;
     EngineInstruments propulsion;
     std::string flightID; // The flight call sign
     std::string destinationAirport;
     std::string originAirport;
     FlightPhase currentPhase;

     double targetThrottle;
     double altitude;
     double heading;
     double airspeed;
     double verticalSpeed;
     double groundSpeed;

     void startEngine() { 
          if (!isEngineStarted) {
               isEngineStarting = true;
               std::cout << "[SYSTEM] Engine start sequence initiated." << std::endl;
          }
     }

     bool getIsEngineStarted() const { return isEngineStarted; }

     // Explicit state transition interace is inline with (AUTOSAR Runnable / API)
     void disconnectTug() {
          if (currentPhase == FlightPhase::PUSHBACK) {
               currentPhase = FlightPhase::TAXI;
               groundSpeed = 0.0;
               std::cout << "[SYSTEM] Tug disconnected. Aircraft is now in TAXI phase." << std::endl;
          }
     }

public:

     Aircraft(std::string tailNum, std::string flightID, std::string destAport, std::string originAport);

     void updatePhysics(double deltaTime, double windSpeed, double windDirection);

     // Getters to feed your JSON telemetry marshaling loop

     const EngineInstruments& getPropulsion() const { return propulsion; }
     const FuelSystem& getFuel() const { return fuel; }
     const Coordinates& getPosition() const { return position; }
     std::string getFlightID() const { return flightID; }

private:
     bool isEngineStarting = false;
     bool isEngineStarted = false;
     FlightPhase currentPhase = FlightPhase::GATE;
};
#endif
