#include "BOSCH_x412.hpp"

// Constructor
BOSCH_x412::BOSCH_x412(int pinNumber, String name, int resistor) {
    this->pinNumber = pinNumber;
    this->name = name;
    this->resistor = resistor;
}

// Destructor
BOSCH_x412::~BOSCH_x412() {}


void BOSCH_x412::init() {
    pinMode(this->pinNumber, INPUT);
}

float BOSCH_x412::readVoltage() {
    return analogRead(this->pinNumber) * (3.3 / 1023.0); //volts
}

float BOSCH_x412::calculateResistence(float voltage) {
    const float VCC = 5.0; //volts
    return (this->resistor) * voltage / (VCC - voltage); //(ohms)
}

float BOSCH_x412::readTemperature() {
    float voltage = this->readVoltage();
    float resistance = this->calculateResistence(voltage);
    //regressão exponencial T= log(B, (R/A))
    const float A = 6636.53;
    const float B = 0.957796;
    float temperature = log(resistance / A) / log(B);
    return temperature;
}