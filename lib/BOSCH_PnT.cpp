#include "BOSCH_PnT.hpp"

// Constructor
BOSCH_PnT::BOSCH_PnT(int pinNumber, String name, int resistor) {
    this->pinNumber = pinNumber;
    this->name = name;
    this->resistor = resistor;
}

// Destructor
BOSCH_PnT::~BOSCH_PnT() {}

