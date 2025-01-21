#ifndef BOSCH_PNT_HPP
#define BOSCH_PNT_HPP

#include <Arduino.h>

class BOSCH_PnT
{
private:
    /* data */
    int pinTemp;
    int pinPressure; 

    String nameTemp;
    String namePressure;

    int resistor; //8200 ohms valor nominal
    
    int Vdivider1; //22000 ohms valor nominal
    int Vdivider2; //10k ohms valor nominal

    float readVoltage();
    float calculateResistence(float voltage);

public:
    BOSCH_PnT(int pinNumber, String name, int resistor);
    ~BOSCH_PnT();
    void init();
    float readTemperature();

};

BOSCH_PnT::BOSCH_PnT(int pinNumber, String name, int resistor) : pinNumber(pinNumber), name(name), resistor(resistor)
{
}

BOSCH_PnT::~BOSCH_PnT()
{
}





#endif