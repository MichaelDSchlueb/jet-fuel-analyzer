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
	flightID(flightID), destinationAirport(destAport), originAirport(originAport), currentPhase(FlightPhase::GATE) {

 };

void Aircraft::updatePhysics(double deltaTime, double windSpeed, double windDirection) {
	if (isEngineStarting && !isEngineStarted) {
		// Starter motor / air compressor spools turbine up to Ground Idle (~22% RPM)
		if (propulsion.coreRPM < 22.0) {
			propulsion.coreRPM += 2.0 * deltaTime;
			propulsion.egt += 15.0 * deltaTime;
			propulsion.oilPressure += 3.0 * deltaTime;
		} else {
			propulsion.coreRPM = 22.0;
			isEngineStarted = true;
			isEngineStarting = false;
			std::cout << "Engine started. Ground Idle 22%." << std::endl;
		}
	}

	if (currentPhase == FlightPhase::PUSHBACK) {
		// Set the target throttle to 0.0 when the aircraft is in the PUSHBACK phase
		groundSpeed = 3.0; // knots
		position.longitude -= 0.00001 * deltaTime;
		position.latitude -= 0.00001 * deltaTime;

		return;

	}

	// calculate RPM Gap
    double gap = targetThrottle - propulsion.coreRPM;

        // Spool engine
    propulsion.coreRPM += (gap * deltaTime * SPOOL_CONSTANT);

	// There is no way to account for targetThrottle being moved up and down in a single tick. This is a limitation of the current physics model.
	// Calculate airspeed
    airspeed = propulsion.coreRPM * THROTTLE_COEFFICIENT;

	if (currentPhase == FlightPhase::CLIMB || currentPhase == FlightPhase::CRUISE) {
		double excessThrust = propulsion.coreRPM - 65.0;

		if (excessThrust > 0.0 && airspeed > 100.0) {
			double altitudePenalty = std::max(0.1, 1.0 - (altitude / 10000.0));

			verticalSpeed = (excessThrust * THRUST_CONSTANT * 40.0) * (airspeed / 150.0) * altitudePenalty;
		} else {
			verticalSpeed = 0.0;
		}
	} else {
		verticalSpeed = 0.0;
	}

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

	altitude += (climbPerSecond * deltaTime);

	double thrustAboveIdle;

	if (currentPhase == FlightPhase::TAXI) {
		// If the aircraft is in the TAXI phase, limit the ground speed to a maximum of 20 knots
		// calculate the thrust force
		thrustAboveIdle = propulsion.coreRPM - 22.0;

		if (groundSpeed == 0.0) {
			if (thrustAboveIdle < STATIC_FRICTION_THRESHOLD) {
				position.velocityN = 0.0;
				position.velocityE = 0.0;
				return;
			} else {
				std::cout << "[SYSTEM] Breaking static friction. Aircraft is now moving." << std::endl;
				groundSpeed = 1.0; // Initial movemenet speed upon breakaway
			}
		}
	}

	if (groundSpeed > 30.0 && targetThrottle < 70.0) {
		groundSpeed = 30.0; // Limit ground speed to 30 knots when throttle is below 70%
	}

	if (propulsion.coreRPM >= 80.0) {
		currentPhase = FlightPhase::TAKEOFF_ROLL;
		std::cout << "[SYSTEM] Aircraft is now in TAKEOFF_ROLL phase." << std::endl;
	}

	if (airspeed >= 140.0) {
		currentPhase = FlightPhase::CLIMB;
		std::cout << "[SYSTEM] Aircraft is now in CLIMB phase." << std::endl;
	}

	
	// It's groundSpeed is moving in the opposite direction of where the nose is pointing
	double aircraftNorth = airspeed * std::cos(heading);
	
	double aircraftEast = airspeed * std::sin(heading);

	double windNorth = windSpeed * std::cos(windDirection);

	double windEast = windSpeed * std::sin(windDirection);

	position.velocityN = aircraftNorth + windNorth;

	position.velocityE = aircraftEast + windEast;

	groundSpeed = std::sqrt(std::pow(position.velocityN,2) + std::pow(position.velocityE,2));

	
	position.latitude = position.latitude + (position.velocityN * deltaTime * 0.00001);
	position.longitude = position.longitude + (position.velocityE * deltaTime * 0.00001);	

}
