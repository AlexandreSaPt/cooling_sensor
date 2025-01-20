#ifndef BOSCH_X412_HPP
#define BOSCH_X412_HPP

#include <Arduino.h>

class BOSCH_x412
{
private:
    /* data */
    int pinNumber;
    String name;
    int resistor; //8200 ohms valor nominal

    float readVoltage();
    float calculateResistence(float voltage);

public:
    BOSCH_x412(int pinNumber, String name, int resistor);
    ~BOSCH_x412();
    void init();
    float readTemperature();

};

BOSCH_x412::BOSCH_x412(int pinNumber, String name, int resistor) : pinNumber(pinNumber), name(name), resistor(resistor)
{
}

BOSCH_x412::~BOSCH_x412()
{
}





#endif