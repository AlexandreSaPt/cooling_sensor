#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

File csv_file;

#define DEBUG 1
#define ANALOG_READ_RESOLUTION 12
#define VCC_board 5

#define FILE_NAME "data.csv" //default "data.csv"

//-----------------STRUCT---------------

/**
 * @brief BOSCH_x039
 * @note Temperature Sensor [can read from -5ºC to 140ºC]
 * @param name
 * @param pinNumber
 * @param resistor is 4.6k nominal (to be read experimentally)
 */
typedef struct BOSCH_x039{
    String name;
    int pinNumber;
    int resistor; //4.6k nominal
};

/**
 * @brief BOSCH_x412
 * @note Temperature Sensor [can read from -5ºC to 140ºC]
 * @param name
 * @param pinNumber
 * @param resistor is 4.6k nominal (to be read experimentally)
 */
typedef struct BOSCH_x412{
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
typedef struct BOSCH_PnT{
    String nameTemp;
    String namePressure;

    int pinTemp;
    int pinPressure;

    int tempResistor; //8.2k

    int Raux; //10k nominal
    int Rread; //22k nominal
};

//-----------------FUNCTIONS---------------

/**
 * @brief Blinks builtin LED in case of an error
 */
void errorLed();

//----RELATED TO SD CARD----
/**
 * @brief Creates a CSV string
 * @param data is the array of data, in order
 * @param size is the size of the array
 * @return the CSV string
 */
String createCSV_string(float data[], int size);

/**
 * @brief Creates a CSV string
 * @param data is the array of string data, in order
 * @param size is the size of the array
 * @return the CSV string
 */
String createCSV_string(String data[], int size);

/**
 * @brief Sends the CSV string to the SD card
 * @param str is the CSV string
 */
void send_toSD(String str);

/**
 * @brief Sends the headers to the SD card
 * @param listHeaders is the list of headers
 * @param size is the size of the list
 * @note the listHeaders must be in the same order as the data
 */
void sendHeaders(String listHeaders[], int size);

/**
 * @brief Gets the data from the sensors
 * @param listSensors is the list of sensors data
 */
void getSensorsData(float listSensors[]);

/**
 * @brief Gets the headers from the sensors
 * @param listHeaders is the list of headers
 * @note the return of the func will be in the param
 */
void getHeaders(String listHeaders[]);


//----RELATED TO SENSORS----
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
float calcTemp(float NTCResistence);

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
        pinMode(list_039[i].pinNumber, INPUT);
    }
    for (int i = 0; i < BOSCH_x412_LENGTH; i++)
    {
        pinMode(list_412[i].pinNumber, INPUT);
    }
    for (int i = 0; i < BOSCH_PnT_LENGTH; i++)
    {
        pinMode(list_PnT[i].pinTemp, INPUT);
        pinMode(list_PnT[i].pinPressure, INPUT);
    }
    pinMode(LED_BUILTIN, HIGH); //for the error led
    
    analogReadResolution(ANALOG_READ_RESOLUTION);

    if(DEBUG){
        Serial.begin(9600);
        while (!Serial) {
            ; // wait for serial port to connect. Needed for native USB port only
        }
    }

    if (!SD.begin(BUILTIN_SDCARD)) {
        error = true;

        if(DEBUG){        
            Serial.println("initialization of the SD failed!");
        }
    }
    if(DEBUG) Serial.println("initialization of the SD done.");

    String listHeaders[BOSCH_x039_LENGTH + BOSCH_x412_LENGTH + BOSCH_PnT_LENGTH];

    getHeaders(listHeaders);
    sendHeaders(listHeaders, BOSCH_x039_LENGTH + BOSCH_x412_LENGTH + BOSCH_PnT_LENGTH);
}

void loop(){
    float listSensors[BOSCH_x039_LENGTH + BOSCH_x412_LENGTH + BOSCH_PnT_LENGTH];

    getSensorsData(listSensors); //load listSensors with the data

    String csv_line = createCSV_string(listSensors, BOSCH_x039_LENGTH + BOSCH_x412_LENGTH + BOSCH_PnT_LENGTH);

    send_toSD(csv_line);

    if(error){
        while(1){
            errorLed();
        }
    }

}

void getHeaders(String listHeaders[]){
    int counter = 0;
    for(int i = 0; i < BOSCH_x039_LENGTH; i++){
        listHeaders[counter] = list_039[i].name;
        counter++;
    }

    for(int i = 0; i < BOSCH_x412_LENGTH; i++){
        listHeaders[counter] = list_412[i].name;
        counter++;
    }

    for(int i = 0; i < BOSCH_PnT_LENGTH; i++){
        listHeaders[counter] = list_PnT[i].nameTemp;
        counter++;

        listHeaders[counter] = list_PnT[i].namePressure;
        counter++;
    }
}


void sendHeaders(String listHeaders[], int size){
    String csv_line = createCSV_string(listHeaders, size);
    send_toSD(csv_line);
}

String createCSV_string(String data[], int size){
    //tested online
    String str = "";
    for(size_t i = 0; i < size - 1; i ++){
    str += String(data[i]);
    str += ";";
    }
    str += String(data[size - 1]);
    str += "\n";

    return str;
}

String createCSV_string(float data[], int size){
    //tested online
    String str = "";
    for(size_t i = 0; i < size - 1; i ++){
    str += String(data[i]);
    str += ";";
    }
    str += String(data[size - 1]);
    str += "\n";

    return str;
}


void errorLed(){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}

void send_toSD(String csv_line){
    //I open and close the file everytime I write because im afraid that if something happens like cuting the power will result in lost data
    csv_file = SD.open(FILE_NAME, FILE_WRITE);
    if(!csv_file){
        error = true;
    }else{
        csv_file.print(csv_line);
        csv_file.close();
    }
  
}




float readVoltage(int pinNumber){
    return ((float) analogRead(pinNumber) * 3.3) / (2^ANALOG_READ_RESOLUTION);
}

float calcNTC(float voltage, int Rdivider){ 
    return voltage * Rdivider / (VCC_board - voltage);
}

float calcTemp(float NTCResistence){
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

void getSensorsData(float listSensors[]){
    int counter = 0;
    for(int i = 0; i < BOSCH_x039_LENGTH; i++){
        float voltage = readVoltage(list_039[i].pinNumber); //volts
        float R_NTC = calcNTC(voltage, list_039[i].resistor); //ohms
        float temperature = calcTemp(R_NTC); //ºC

        listSensors[counter] = temperature;
        counter++;

        if(DEBUG) continue;
        Serial.print(list_039[i].name);
        Serial.print(" - ");
        Serial.print(temperature);
        Serial.println(" C");
    }

    for(int i = 0; i < BOSCH_x412_LENGTH; i++){
        float voltage = readVoltage(list_412[i].pinNumber); //volts
        float R_NTC = calcNTC(voltage, list_412[i].resistor); //ohms
        float temperature = calcTemp(R_NTC); //ºC

        listSensors[counter] = temperature;
        counter++;

        if(DEBUG) continue;
        Serial.print(list_412[i].name);
        Serial.print(" - ");
        Serial.print(temperature);
        Serial.println(" C");
    }

    for(int i = 0; i < BOSCH_PnT_LENGTH; i++){
        float voltageTemp = readVoltage(list_PnT[i].pinTemp); //volts
        float R_NTC = calcNTC(voltageTemp, list_PnT[i].tempResistor); //ohms
        float temperature = calcTemp(R_NTC); //ºC

        listSensors[counter] = temperature;
        counter++;

        float Vinit = readVoltage(list_PnT[i].pinPressure); //volts
        float signalVoltage = calcSignalVoltage(Vinit, list_PnT[i].Raux, list_PnT[i].Rread); //volts
        float pressure = calcPressure(signalVoltage); //kPa

        listSensors[counter] = pressure;
        counter++;

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