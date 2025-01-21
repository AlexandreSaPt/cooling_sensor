#include <Arduino.h>
#define DEBUG 1
#define ANALOG_READ_RESOLUTION 12
#define VCC_board 5

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


/**
 * @brief BOSCH_PnT
 * @note Temperature Sensor [can read from -5ºC to 140ºC]
 * @param nameTemp
 * @param namePressure
 * @param pinTemp
 * @param pinPressure
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

/**
 * @brief Sets the pin mode of the sensor
*/ 
void initSensor(BOSCH_PnT sensor);

/**
 * @brief Sets the pin mode of the sensor
 */
void initSensor(BOSCH_x039 sensor);

/**
 * @brief Sets the pin mode of the sensor
 */
void initSensor(BOSCH_x412 sensor);

/**
 * @brief Reads the voltage from the ADC
 * @param pinNumber is the pin number of the port
 * @return the voltage read
 */
float readVoltage(int pinNumber);

/**
 * @brief Calculates the NTC resistance
 * @param voltage is the voltage read from the ADC
 * @param Rdivider is the resistor value of the voltage divider
 * @return the NTC resistance
 */
float calcNTC(float voltage, int Rdivider);

/**
 * @brief Calculates the temperature
 * @param NTCResistence is the NTC resistance
 * @return the temperature
 * @note T= log(B, (R/A))
 */
float calTemp(float NTCResistence);

/**
 * @brief Calculates the signal voltage
 * @param Vread is the voltage read from the ADC
 * @param Raux is the auxiliary resistor
 * @param Rread is the resistor in which was read the voltage
 * @return the initial signal voltage
 */
float calcSignalVoltage(float Vread, int Raux, int Rread);

/**
 * @brief Calculates the pressure
 * @param signalVoltage is the initial signal voltage
 * @return the pressure
 * @note y = mx + b, in the datasheet
 */
float calcPressure(float signalVoltage);



#define BOSCH_x039_LENGTH 4
const BOSCH_x039 BOSCH_x039_1 = {"BOSCH_x039_1", A4, 4600};
const BOSCH_x039 BOSCH_x039_2 = {"BOSCH_x039_2", A5, 4600};
const BOSCH_x039 BOSCH_x039_3 = {"BOSCH_x039_3", A6, 4600};
const BOSCH_x039 BOSCH_x039_4 = {"BOSCH_x039_4", A7, 4600};

const BOSCH_x039 list_039[BOSCH_x039_LENGTH] = {BOSCH_x039_1, BOSCH_x039_2, BOSCH_x039_3, BOSCH_x039_4};

#define BOSCH_x412_LENGTH 2
const BOSCH_x412 BOSCH_x412_1 = {"BOSCH_x412_1", A2, 4600};
const BOSCH_x412 BOSCH_x412_2 = {"BOSCH_x412_2", A3, 4600};

const BOSCH_x412 list_412[2] = {BOSCH_x412_1, BOSCH_x412_2};

#define BOSCH_PnT_LENGTH 6
const BOSCH_PnT BOSCH_PnT_1 = {"PnT_Temperature_1", "PnT_Pressure_1", A14, A13, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_2 = {"PnT_Temperature_2", "PnT_Pressure_2", A15, A12, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_3 = {"PnT_Temperature_3", "PnT_Pressure_3", A16, A11, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_4 = {"PnT_Temperature_4", "PnT_Pressure_4", A17, A10, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_5 = {"PnT_Temperature_5", "PnT_Pressure_5", A0, A8, 8200, 10000, 22000};
const BOSCH_PnT BOSCH_PnT_6 = {"PnT_Temperature_6", "PnT_Pressure_6", A1, A9, 8200, 10000, 22000};

const BOSCH_PnT list_PnT[6] = {BOSCH_PnT_1, BOSCH_PnT_2, BOSCH_PnT_3, BOSCH_PnT_4, BOSCH_PnT_5, BOSCH_PnT_6};


bool error = false;

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

    analogReadResolution(ANALOG_READ_RESOLUTION);

    if(DEBUG){
        Serial.begin(9600);
    }
}

void loop(){
    for(int i = 0; i < BOSCH_x039_LENGTH; i++){
        float voltage = readVoltage(list_039[i].pinNumber); //volts
        float R_NTC = calcNTC(voltage, list_039[i].resistor); //ohms
        float temperature = calTemp(R_NTC); //ºC

        if(DEBUG) continue;
        Serial.print(list_039[i].name);
        Serial.print(" - ");
        Serial.print(temperature);
        Serial.println(" C");
    }

    for(int i = 0; i < BOSCH_x412_LENGTH; i++){
        float voltage = readVoltage(list_412[i].pinNumber); //volts
        float R_NTC = calcNTC(voltage, list_412[i].resistor); //ohms
        float temperature = calTemp(R_NTC); //ºC


        if(DEBUG) continue;
        Serial.print(list_412[i].name);
        Serial.print(" - ");
        Serial.print(temperature);
        Serial.println(" C");
    }

    for(int i = 0; i < BOSCH_PnT_LENGTH; i++){
        float voltageTemp = readVoltage(list_PnT[i].pinTemp); //volts
        float R_NTC = calcNTC(voltageTemp, list_PnT[i].tempResistor); //ohms
        float temperature = calTemp(R_NTC); //ºC

        float Vinit = readVoltage(list_PnT[i].pinPressure); //volts
        float signalVoltage = calcSignalVoltage(Vinit, list_PnT[i].Raux, list_PnT[i].Rread); //volts
        float pressure = calcPressure(signalVoltage); //kPa

        if(DEBUG) continue;
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
    return ((float) analogRead(pinNumber) * 3.3) / (2^ANALOG_READ_RESOLUTION);
}

float calcNTC(float voltage, int Rdivider){ 
    return voltage * Rdivider / (VCC_board - voltage);
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
