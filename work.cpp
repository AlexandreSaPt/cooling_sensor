#include <Arduino.h>
#define DEBUG 1

/**
 * @brief BOSCH_x039
 * @note Temperature Sensor [can read from -5ºC to 140ºC]
 * @param resistor is 4.6k nominal (to be read experimentally)
 */
struct BOSCH_x039{
    String name;
    int pinNumber;
    int resistor; //4.6k nominal
};

//Temperature Sensor [can read from -5ºC to 140ºC]

/**
 * @brief BOSCH_x412
 * @note Temperature Sensor [can read from -5ºC to 140ºC]
 * @param resistor is 4.6k nominal (to be read experimentally)
 */
struct BOSCH_x412{
    String name;
    int pinNumber;
    int resistor; //4.6k nominal
};

//Temperature Sensor [can read from -5ºC to 140ºC]
//Pressure Sensor [can read from 10kPa to 300kPa]

/**
 * @brief BOSCH_PnT
 * @note Temperature Sensor [can read from -5ºC to 140ºC]
 * @param tempResistor is 8.2k nominal (to be read experimentally)
 * @note Pressure Sensor [can read from 10kPa to 300kPa]
 * @param Raux is 10k nominal (to be read experimentally)
 * @param Rread is 22k nominal (to be read experimentally)
 */
struct BOSCH_PnT{
    String nameTemp;
    String namePressure;

    int pinTemp;
    int pinPressure;

    int tempResistor; //8.2k

    int Raux; //10k nominal
    int Rread; //22k nominal
};

void initSensor(BOSCH_PnT sensor);
void initSensor(BOSCH_x039 sensor);
void initSensor(BOSCH_x412 sensor);


float readVoltage(int pinNumber);
float calcNTC(float voltage, int Rdivider);
float calTemp(float NTCResistence);


float calcSignalVoltage(float Vread, int r1, int r2);
float calcPressure(float signalVoltage);

#define BOSCH_x039_LENGTH 4
const BOSCH_x039 BOSCH_x039_1 = {"BOSCH_x039_1", A0, 4600};
const BOSCH_x039 BOSCH_x039_2 = {"BOSCH_x039_2", A13, 4600};
const BOSCH_x039 BOSCH_x039_3 = {"BOSCH_x039_3", 1, 4600};
const BOSCH_x039 BOSCH_x039_4 = {"BOSCH_x039_4", 1, 4600};

const BOSCH_x039 list_039[BOSCH_x039_LENGTH] = {BOSCH_x039_1, BOSCH_x039_2, BOSCH_x039_3, BOSCH_x039_4};

#define BOSCH_x412_LENGTH 2
const BOSCH_x412 BOSCH_x412_1 = {"BOSCH_x412_1", 1, 4600};
const BOSCH_x412 BOSCH_x412_2 = {"BOSCH_x412_2", 1, 4600};

const BOSCH_x412 list_412[2] = {BOSCH_x412_1, BOSCH_x412_2};

#define BOSCH_PnT_LENGTH 6
const BOSCH_PnT BOSCH_PnT_1 = {"PnT_Temperature_1", "PnT_Pressure_1", 1, 1, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_2 = {"PnT_Temperature_2", "PnT_Pressure_2", 1, 1, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_3 = {"PnT_Temperature_3", "PnT_Pressure_3", 1, 1, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_4 = {"PnT_Temperature_4", "PnT_Pressure_4", 1, 1, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_5 = {"PnT_Temperature_5", "PnT_Pressure_5", 1, 1, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_6 = {"PnT_Temperature_6", "PnT_Pressure_6", 1, 1, 8200, 10000, 22000};

const BOSCH_PnT list_PnT[6] = {BOSCH_PnT_1, BOSCH_PnT_2, BOSCH_PnT_3, BOSCH_PnT_4, BOSCH_PnT_5, BOSCH_PnT_6};


void setup(){
    for (int i = 0; i < BOSCH_x039_LENGTH; i++)
    {
        initSensor(list_039[i]);
    }
    for (int i = 0; i < BOSCH_x412_LENGTH; i++)
    {
        initSensor(list_412[i]);
    }
    for (int i = 0; i < BOSCH_PnT_LENGTH; i++)
    {
        initSensor(list_PnT[i]);
    }
    
    analogReadResolution(12);
}

void loop(){
    for(int i = 0; i < BOSCH_x039_LENGTH; i++){
        float voltage = readVoltage(list_039[i].pinNumber);
        float R_NTC = calcNTC(voltage, list_039[i].resistor);
        float temperature = calTemp(R_NTC);

        Serial.print(list_039[i].name);
        Serial.print(" - ");
        Serial.print(temperature);
        Serial.println(" C");
    }

    for(int i = 0; i < BOSCH_x412_LENGTH; i++){
        float voltage = readVoltage(list_412[i].pinNumber);
        float R_NTC = calcNTC(voltage, list_412[i].resistor);
        float temperature = calTemp(R_NTC);

        Serial.print(list_412[i].name);
        Serial.print(" - ");
        Serial.print(temperature);
        Serial.println(" C");
    }

    for(int i = 0; i < BOSCH_PnT_LENGTH; i++){
        float voltageTemp = readVoltage(list_PnT[i].pinTemp);
        float R_NTC = calcNTC(voltageTemp, list_PnT[i].tempResistor);
        float temperature = calTemp(R_NTC);

        float Vinit = readVoltage(list_PnT[i].pinPressure);
        float signalVoltage = calcSignalVoltage(Vinit, list_PnT[i].Raux, list_PnT[i].Rread);
        float pressure = calcPressure(signalVoltage);

        Serial.print(list_PnT[i].nameTemp);
        Serial.print(" - ");
        Serial.print(temperature);
        Serial.println(" C");

        Serial.print(list_PnT[i].namePressure);
        Serial.print(" - ");
        Serial.print(pressure);
        Serial.println(" Pa");
    }

}



float readVoltage(int pinNumber){
    return analogRead(pinNumber);
}

float calcNTC(float voltage, int Rdivider){
    const int VCC = 5;
    return voltage * Rdivider / (VCC - voltage);
}

float calTemp(float NTCResistence){
    //regressão logaritmica T= log(B, (R/A))
    const float A = 6636.53;
    const float B = 0.957796;
    float temperature = log(NTCResistence / A) / log(B);
    return temperature;
}

float calcSignalVoltage(float Vread, int Raux, int Rread){
    return Vread * (Raux + Rread) / Rread;
}

float calcPressure(float signalVoltage){
    const float m = 68.2353;
    const float b = -17.2941;

    return m * signalVoltage + b;
}

void initSensor(BOSCH_PnT sensor){
    pinMode(sensor.pinTemp, INPUT);
    pinMode(sensor.pinPressure, INPUT);
}
void initSensor(BOSCH_x039 sensor){
    pinMode(sensor.pinNumber, INPUT);
}
void initSensor(BOSCH_x412 sensor){
    pinMode(sensor.pinNumber, INPUT);
}
