// Aircraft.cpp
#include "Aircraft.h"
#include <cmath>
#include <cmath>

const double THROTTLE_COEFFICIENT = 0.05;
const double GRAVITY_COEFFICIENT = 0.12;
const double SPOOL_CONSTANT = 2.5;
const double ALTITUDE_EFFICIENCY_BASE = 1.2;
const double THRUST_CONSTANT = 1.0;
const double STATIC_FRICTION_THRESHOLD = 10.0;

// Aircraft constructor
Aircraft::Aircraft(std::string tailNum, std::string flightID, std::string destAport, std::string originAport) : tailNumber(tailNum), 
	flightID(flightID), destinationAirport(destAport), originAirport(originAport), currentPhase(FlightPhase::PUSHBACK) {

 };

void Aircraft::updatePhysics(double deltaTime, double windSpeed, double windDirection) {
        if (currentPhase != FlightPhase::PUSHBACK) {
	// calculate RPM Gap
        	double gap = targetThrottle - propulsion.coreRPM;

        // Spool engine
        	propulsion.coreRPM = propulsion.coreRPM + (gap * deltaTime * SPOOL_CONSTANT);

	// Calculate airspeed
        	airspeed = propulsion.coreRPM * THROTTLE_COEFFICIENT;

	// caclculate exhaustGasTemp
        	propulsion.egt = (propulsion.coreRPM * 10.0) + (verticalSpeed * .05);

	// Get the base burn
        	double baseBurn = propulsion.coreRPM * THROTTLE_COEFFICIENT;

	// calculate the climb penalty
        	double climbPenalty = verticalSpeed * GRAVITY_COEFFICIENT;

	// get the burn rate based on baseBurn and climbPenalty
		fuel.burnRate = (baseBurn + climbPenalty) / ALTITUDE_EFFICIENCY_BASE;

	// calculate reamining fuel
		fuel.remaining = fuel.remaining - (fuel.burnRate * deltaTime);

	// climbs per second
		double climbPerSecond = verticalSpeed/60.0;

		altitude = altitude + (climbPerSecond * deltaTime);
	};

	double aircraftNorth = airspeed * std::cos(heading);
	
	double aircraftEast = airspeed * std::sin(heading);

	double windNorth = windSpeed * std::cos(windDirection);

	double windEast = windSpeed * std::sin(windDirection);

	position.velocityN = aircraftNorth + windNorth;

	position.velocityE = aircraftEast + windEast;

	groundSpeed = std::sqrt(std::pow(position.velocityN,2) + std::pow(position.velocityE,2));

	//if currentPhase != "pushback":
		//if groundSpeed < STATIC_FRICTION_THRESHOLD:
			// position.groundSpeed = 0.0
			// position.velocityN = 0.0
			// poisition.veloctyE = 0.0
	position.latitude = position.latitude + (position.velocityN * deltaTime * 0.00001);
	position.longitude = position.longitude + (position.velocityE * deltaTime * 0.00001);	

}
