#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

File data_file;

//----------sensor pins--------------
#define BOSCHx2_A A14
#define BOSCHx2_B A15

#define BOSCHx4_A A16
#define BOSCHx4_B A17
#define BOSCHx4_C A13
#define BOSCHx4_D A12

#define BOSCH_pressure_A A10
#define BOSCH_temp_A A11

#define BOSCH_pressure_B A1
#define BOSCH_temp_B A0

#define BOSCH_pressure_C A3
#define BOSCH_temp_C A2

#define BOSCH_pressure_D A5
#define BOSCH_temp_D A4

#define BOSCH_pressure_E A7
#define BOSCH_temp_E A6

#define BOSCH_pressure_F A9
#define BOSCH_temp_F A8

#define NUMBER_OF_SIGNALS 18

String createCSV_string(int data[], int size);
void send_toSD(String str);
void errorLed();

void printDirectory(File dir, int numTabs);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(BOSCHx2_A, INPUT);
  pinMode(BOSCHx2_B, INPUT);

  pinMode(BOSCHx4_A, INPUT);
  pinMode(BOSCHx4_B, INPUT);
  pinMode(BOSCHx4_C, INPUT);
  pinMode(BOSCHx4_D, INPUT);

  pinMode(BOSCH_pressure_A, INPUT);
  pinMode(BOSCH_temp_A, INPUT);

  pinMode(BOSCH_pressure_B, INPUT);
  pinMode(BOSCH_temp_B, INPUT);

  pinMode(BOSCH_pressure_C, INPUT);
  pinMode(BOSCH_temp_C, INPUT);

  pinMode(BOSCH_pressure_D, INPUT);
  pinMode(BOSCH_temp_D, INPUT);

  pinMode(BOSCH_pressure_E, INPUT);
  pinMode(BOSCH_temp_E, INPUT);

  pinMode(BOSCH_pressure_F, INPUT);
  pinMode(BOSCH_temp_F, INPUT);

  pinMode(LED_BUILTIN, OUTPUT); //for debugging purpose

  analogReadResolution(12);


  if (!SD.begin(BUILTIN_SDCARD)) {
    //Initialization of SD card failed
    while (1){
      //starts blinking
      Serial.println("SD card failed");
      errorLed();
    }
  }

  Serial.print("Total size: ");
  Serial.println(SD.totalSize());

  Serial.print("Used size: ");
  Serial.println(SD.usedSize());

  Serial.print("Free space: ");
  Serial.println(SD.totalSize() - SD.usedSize());

  File my_file = SD.open("csv.txt");

  

  //File root = SD.open("/");
  //printDirectory(root, 0);

  //Send the data headers
  String header = "BOSCHx2_A;BOSCHx2_B;BOSCHx4_A;BOSCHx4_B;BOSCHx4_C;BOSCHx4_D;BOSCH_pressure_A;BOSCH_temp_A;BOSCH_pressure_B;BOSCH_temp_B;BOSCH_pressure_C;BOSCH_temp_C;BOSCH_pressure_D;BOSCH_temp_D;BOSCH_pressure_E;BOSCH_temp_E;BOSCH_pressure_F;BOSCH_temp_F\n";
  send_toSD(header);
  while (my_file.available()) {
    Serial.println("Content");
    Serial.write(data_file.read());
  }
}

void loop() {
  //read from sensors
  delay(100);
  int temp_X2_A = analogRead(BOSCHx2_A);
  int temp_X2_B = analogRead(BOSCHx2_B);
  
  int temp_X4_A = analogRead(BOSCHx4_A);
  int temp_X4_B = analogRead(BOSCHx4_B);
  int temp_X4_C = analogRead(BOSCHx4_C);
  int temp_X4_D = analogRead(BOSCHx4_D);

  int temp_pressure_A = analogRead(BOSCH_pressure_A);
  int temp_temp_A = analogRead(BOSCH_temp_A);

  int temp_pressure_B = analogRead(BOSCH_pressure_B);
  int temp_temp_B = analogRead(BOSCH_temp_B);

  int temp_pressure_C = analogRead(BOSCH_pressure_C);
  int temp_temp_C = analogRead(BOSCH_temp_C);

  int temp_pressure_D = analogRead(BOSCH_pressure_D);
  int temp_temp_D = analogRead(BOSCH_temp_D);

  int temp_pressure_E = analogRead(BOSCH_pressure_E);
  int temp_temp_E = analogRead(BOSCH_temp_E);

  int temp_pressure_F = analogRead(BOSCH_pressure_F);
  int temp_temp_F = analogRead(BOSCH_temp_F);

  int data_per_frame[] = {temp_X2_A, temp_X2_B, temp_X4_A, temp_X4_B, temp_X4_C, temp_X4_D, temp_pressure_A, temp_temp_A, temp_pressure_B, temp_temp_B, temp_pressure_C, temp_temp_C, temp_pressure_D, temp_temp_D, temp_pressure_E, temp_temp_E, temp_pressure_F, temp_temp_F};

  String csv_line = createCSV_string(data_per_frame, NUMBER_OF_SIGNALS);

  //send to sd card
  send_toSD(csv_line);
}


String createCSV_string(int data[], int size){
  //tested online
  String str = "";
  for(int i = 0; i < size - 1; i ++){
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
      Serial.println("Error opening file");
      errorLed();
    }
  }
  data_file.print(csv_line);
  data_file.close();
}


void printDirectory(File dir, int numTabs) {

  while (true) {

    File entry =  dir.openNextFile();

    if (! entry) {

      // no more files

      break;

    }

    for (uint8_t i = 0; i < numTabs; i++) {

      Serial.print('\t');

    }

    Serial.print(entry.name());

    if (entry.isDirectory()) {

      Serial.println("/");

      printDirectory(entry, numTabs + 1);

    } else {

      // files have sizes, directories do not

      Serial.print("\t\t");

      Serial.println(entry.size(), DEC);

    }

    entry.close();

  }
}