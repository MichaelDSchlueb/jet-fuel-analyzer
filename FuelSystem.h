#ifndef FUELSYSTEM_H
#define FUELSYSTEM_H

#include <iostream>
struct FuelSystem {

      double capacity;
      double remaining; // Previous remaining fuel - fuel burned in this tick
      double burnRate;

};
#endif
