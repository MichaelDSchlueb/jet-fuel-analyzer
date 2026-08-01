#ifndef ENGINEINSTRUMENTS_H
#define ENGINEINSTRUMENTS_H

#include <iostream>

struct EngineInstruments {

      public:
      	double coreRPM; // This is a percentage. Must be inserted as XX.X%
      	double egt; // In degrees celsius
      	double oilPressure; // As PSI
	EngineInstruments(double coreRPM, double egt, double oilPressure);

	EngineInstruments(): coreRPM(0.0), egt(0.0), oilPressure(0.0) {}

};
#endif // ENGINEINSTRUMENTS_H
