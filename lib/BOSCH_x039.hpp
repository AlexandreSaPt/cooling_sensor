#ifndef BOSCH_X039_HPP
#define BOSCH_X039_HPP

#include <Arduino.h>

class BOSCH_x039
{
private:
    /* data */
    int pinNumber;
    String name;
    int resistor; //4600 omhs valor nominal

    float readVoltage();
    float calculateResistence(float voltage);

public:
    BOSCH_x039(int pinNumber, String name, int resistor);
    ~BOSCH_x039();
    void init();
    float readTemperature();

};

BOSCH_x039::BOSCH_x039(int pinNumber, String name, int resistor) : pinNumber(pinNumber), name(name), resistor(resistor)
{
}

BOSCH_x039::~BOSCH_x039()
{
}





#endif