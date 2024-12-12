#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

File data_file;

//----------sensor pins--------------
#define BOSCH1_A A9
#define BOSCH1_B A8

#define BOSCH2_A A7
#define BOSCH2_B A6
#define BOSCH2_C A5
#define BOSCH2_D A4

#define NUMBER_OF_SIGNALS 6

String createCSV_string(int data[], int size);
void send_toSD(String str);
void errorLed();

void setup() {
  // put your setup code here, to run once:
  pinMode(BOSCH1_A, INPUT);
  pinMode(BOSCH1_B, INPUT);

  pinMode(BOSCH2_A, INPUT);
  pinMode(BOSCH2_B, INPUT);
  pinMode(BOSCH2_C, INPUT);
  pinMode(BOSCH2_D, INPUT);

  pinMode(LED_BUILTIN, OUTPUT); //for debugging purpose

  analogReadResolution(12);


  if (!SD.begin(BUILTIN_SDCARD)) {
    //Initialization of SD card failed
    while (1){
      //starts blinking
      errorLed();
    }
  }

  //Send the data headers
  send_toSD("Temp1;Temp2;Temp3;Temp4;Temp5;Temp6\n");

}

void loop() {
  //read from sensors
  int temp_value1_A = analogRead(BOSCH1_A);
  int temp_value1_B = analogRead(BOSCH1_B);

  int temp_value2_A = analogRead(BOSCH2_A);
  int temp_value2_B = analogRead(BOSCH2_B);
  int temp_value2_C = analogRead(BOSCH2_C);
  int temp_value2_D = analogRead(BOSCH2_D);


  int data_per_frame[] = {temp_value1_A, temp_value1_B, temp_value2_A, temp_value2_B, temp_value2_C, temp_value2_D};
  String csv_line = createCSV_string(data_per_frame, NUMBER_OF_SIGNALS);

  //send to sd card
  send_toSD(csv_line);
}


String createCSV_string(int data[], int size){
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
  data_file = SD.open("csv.txt");
  if(!data_file){
    while(1){
      //starts blinking
      errorLed();
    }
  }
  data_file.print(csv_line);
  data_file.close();
}


